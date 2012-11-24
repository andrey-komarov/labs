import System.IO
import qualified Data.Set as S
import qualified Data.Map as M
import Control.Applicative
import Data.List
import Debug.Trace

newtype Reader r a = Reader {
    runReader :: r -> a
}

instance Monad (Reader r) where
    return a = Reader $ \_ -> a
    m >>= k  = Reader $ \r -> runReader (k (runReader m r)) r

ask :: Reader r r
ask = Reader $ \r -> r

unique = S.toList . S.fromList

(|.|) :: (a -> Bool) -> (a -> Bool) -> (a -> Bool)
(f |.| g) x = f x || g x

data CFGrammar = CFGrammar
     { start :: !Char
     , states :: ![Char]
     , rules :: Char -> [Result]
     }

data Result
    = Epsilon
    | Rule ![Char] deriving (Show)

type Rule = (Char, Result)

isTerminal :: Char -> Bool
isTerminal ch = 'a' <= ch && ch <= 'z'

rule :: String -> Rule
rule s = (head s, rules) where
    ss = words s
    rules = case length ss of
        2 -> Epsilon
        3 -> Rule $! ss !! 2

allStatesFromResult :: Result -> [Char]
allStatesFromResult r = case r of
    Epsilon -> []
    Rule rx -> rx 

allStatesFromRule :: Rule -> [Char]
allStatesFromRule (c, r) = c:allStatesFromResult r

fromRules :: Char -> [Rule] -> CFGrammar
fromRules s rx = CFGrammar s states go where
    mgo = M.fromListWith (++) $ map (\(x, y) -> (x, [y])) rx 
    go a = (M.findWithDefault [] a mgo)
    states = unique $! s : concatMap allStatesFromRule rx

fixed :: ([Char] -> Reader CFGrammar [Char]) -> [Char] -> Reader CFGrammar [Char]
fixed it s = do
    newS <- it s
    if s == newS
        then return s
        else fixed it newS

producingStep :: [Char] -> Reader CFGrammar [Char]
producingStep s = do
    g <- ask
    return $! unique $! s ++ let 
        producing' s = any producing'' $ rules g s
        producing'' r = case r of 
            Epsilon -> True
            Rule rs -> all ((`elem` s) |.| isTerminal) rs
        in filter producing' (states g)

producing :: Reader CFGrammar [Char]
producing = fixed producingStep []

reachableStep :: [Char] -> Reader CFGrammar [Char]
reachableStep s = do
    g <- ask
    return $! unique $! s ++ 
                        (s >>= \x -> concatMap allStatesFromResult (rules g x))

reachable :: Reader CFGrammar [Char]
reachable = do
    g <- ask
    fixed reachableStep [start g]

useless :: Reader CFGrammar [Char]
useless = do
    g <- ask
    let all = states g
    reachable <- reachable
    producing <- producing
    let bad x = filter (not . (`elem` x)) all
    let unreachable = bad reachable
    let notproducing = bad producing
    return $! unique $! unreachable ++ notproducing

main = do
    inp <- openFile "useless.in" ReadMode
    s <- head <$> last <$> words <$> hGetLine inp
    rules <- map rule <$> lines <$> hGetContents inp
    let g = fromRules s rules
    print $ states g
    writeFile "useless.out" $! intersperse ' ' $! filter (not . isTerminal) $! runReader useless g
