import System.IO
import qualified Data.Map as M
import qualified Data.Set as S
import Data.Monoid
import Control.Applicative
import Data.List

unique :: Ord a => [a] -> [a]
--unique = map head . groupBy (==)
unique = S.toList . S.fromList

rule :: String -> (NonTerminal, Result)
rule ss = (head $ s !! 0, r) where 
    s = words ss
    r = case s !! 2 of
        nt:[] -> Finish nt
        t:nt:[] -> Produce t nt

data AutomataGrammar = AutomataGrammar 
    { start :: !NonTerminal 
    , go :: NonTerminal -> [Result]
    }

type NonTerminal = Char 
type Terminal = Char
data Result
    = Produce !Terminal !NonTerminal
    | Finish !Terminal deriving (Eq)
type Rule = (NonTerminal, Result)

fromRules :: NonTerminal -> [Rule] -> AutomataGrammar
fromRules s rx = AutomataGrammar s go where
    mgo :: M.Map NonTerminal [Result]
    mgo = M.fromListWith (++) $ map (\(x, y) -> (x, [y])) rx 
    go :: NonTerminal -> [Result]
    go a = (M.findWithDefault [] a mgo)

accept :: AutomataGrammar -> [Terminal] -> Bool
accept g s = eatsLast (last s) preLast where 
    preLast :: [NonTerminal]
    preLast = foldl (flip eatOne) [start g] (init s)

    eatsLast :: Terminal -> [NonTerminal] -> Bool
    eatsLast ch w = not $ null $ w >>= (\nt -> filter (== Finish ch) $! go g nt)

    eatOne :: Terminal -> [NonTerminal] -> [NonTerminal]
    eatOne ch w = unique $! map dest $! w >>= (\nt -> filter (withStart ch) $! go g nt)

    dest :: Result -> NonTerminal
    dest (Produce _ s) = s

    withStart :: NonTerminal -> Result -> Bool
    withStart ch (Produce c _) = c == ch
    withStart _ _ = False

solve :: [String] -> String
solve xs = unlines $ map (\b -> if b then "yes" else "no") results where
    n = read $ head $ words $ head xs
    s = last $ head xs 
    rules = take n $ drop 1 xs
    m = read $ head $ drop (n+1) xs
    queries = take m $ drop (n+2) xs
    
    g = fromRules s $ map rule rules
    results = map (accept g) queries

main = do
    inp <- lines <$> readFile "automaton.in"
    writeFile "automaton.out" $ solve inp
