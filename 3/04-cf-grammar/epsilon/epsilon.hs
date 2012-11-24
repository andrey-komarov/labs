import System.IO
import qualified Data.Set as S
import qualified Data.Map as M
import Data.List
import Control.Applicative

newtype Reader r a = Reader {
    runReader :: r -> a
}

instance Monad (Reader r) where
    return a = Reader $ \_ -> a
    m >>= k  = Reader $ \r -> runReader (k (runReader m r)) r

ask :: Reader r r
ask = Reader $ \r -> r

unique :: Ord a => [a] -> [a]
unique = S.toList . S.fromList

type Rule = (Char, Result)

data Result
    = Epsilon
    | Rule ![Char]

data CFGrammar = CFGrammar
    { start :: Char
    , states :: [Char]
    , rules :: Char -> [Result]
    }

it :: [Char] -> Reader CFGrammar [Char]
it s = do
    g <- ask
    return $! unique $! s ++ let 
        epsilon s = any emptyResult $ rules g s
        emptyResult r = case r of 
            Epsilon -> True
            Rule rs -> all (`elem` s) rs
        in filter epsilon (states g)

fixed :: [Char] -> Reader CFGrammar [Char]
fixed s = do
    newS <- it s
    if s == newS
        then return s
        else fixed newS

rule :: String -> Rule
rule s = (head s, rules) where
    ss = words s
    rules = case length ss of
        2 -> Epsilon
        3 -> Rule $! ss !! 2

allStatesFromRule :: Rule -> [Char]
allStatesFromRule (c, r) = case r of
    Epsilon -> [c]
    Rule rx -> c:rx 

fromRules :: Char -> [Rule] -> CFGrammar
fromRules s rx = CFGrammar s states go where
    mgo = M.fromListWith (++) $ map (\(x, y) -> (x, [y])) rx 
    go a = (M.findWithDefault [] a mgo)
    states = unique $! concatMap allStatesFromRule rx

main = do
    inp <- openFile "epsilon.in" ReadMode
    line <- words <$> hGetLine inp
    let s = head $ last line
    rules <- lines <$> hGetContents inp
    let g = fromRules s $! map rule rules
    writeFile "epsilon.out" $! intersperse ' ' $! runReader (fixed []) g
