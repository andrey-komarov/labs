import System.IO
import qualified Data.Set as S
import qualified Data.Map as M
import Control.Applicative
import Data.List
import Data.Array.ST
import Control.Monad.ST
import Data.Int
import Debug.Trace

modulo :: Int64
modulo = 1000000007

unique = S.toList . S.fromList

data NFCGrammar = NFCGrammar
    { start :: !Char
    , states :: ![Char]
    , rules :: Char -> [Result]
    }

data Result
    = Produce {-# UNPACK #-} !Char {-# UNPACK #-} !Char
    | Finish {-# UNPACK #-} !Char

type Rule = (Char, Result)

allStatesFromRule :: Rule -> [Char]
allStatesFromRule (c, r) = [c]

rule :: String -> Rule
rule s = (head s, rules) where
    to = last $! words s
    rules = case length  to of
        1 -> Finish $! head to
        2 -> Produce (head to) $! last to 

fromRules :: Char -> [Rule] -> NFCGrammar
fromRules s rx = NFCGrammar s states go where
    mgo = M.fromListWith (++) $ map (\(x, y) -> (x, [y])) rx 
    go a = (M.findWithDefault [] a mgo)
    states = unique $! concatMap allStatesFromRule rx

waysToAccept :: [Char] -> NFCGrammar -> Int64
waysToAccept s g = runST run where
    run :: ST s Int64
    run = do
        let n = length s
        a <- newArray ((0, 0, 'A'), (n, n, 'Z')) (-1)
        let finishes = do
                        rule <- states g
                        Finish c <- rules g rule
                        return $! ((rule, c), 1)
        let waysToProduceOneLetter = M.fromListWith (+) $! finishes ++ 
                                        [((r, t), 0) | t <- s, r <- states g]
        sequence_ $! do
            (i, c) <- zip [0..] s
            t <- states g
            return $! writeArray a (i, i+1, t) $! waysToProduceOneLetter M.! (t, c)
        dp a 0 (start g) s
    
    turns :: [Char] -> [(Int, [Char], [Char])]
    turns xs = init $! tail $! zip3 [0..] (inits xs) (tails xs)

    dp :: STUArray s (Int, Int, Char) Int64 -> Int -> Char -> [Char] -> ST s Int64
    dp d pos rule s | d `seq` pos `seq` rule `seq` s `seq` False = undefined
    dp d pos rule s = do
        let n = length s
        val <- readArray d (pos, pos + n, rule)
        if val >= 0
            then return val
            else do
                let t = turns s
                let rulesFromMe = do
                                    Produce c1 c2 <- rules g rule
                                    return $! (c1, c2) 
                firsts <- sequence [dp d pos r' s1 | (_, s1, _) <- t, (r', _) <- rulesFromMe]
                seconds <- sequence [dp d (pos+i) r' s2 | (i, _, s2) <- t, (_, r') <- rulesFromMe]
                let ans = sum (zipWith (\a b -> a * b `mod` modulo) firsts seconds) `mod` modulo 
                writeArray d (pos, pos + n, rule) $! ans
                return ans

main = do
    inp <- openFile "nfc.in" ReadMode
    firstLine <- words <$> hGetLine inp
    let n = read $! firstLine !! 0
    let s = head $! firstLine !! 1
    rules <- map rule <$> sequence [hGetLine inp | _ <- [1..n]]
    let g = fromRules s rules
    line <- hGetLine inp
    writeFile "nfc.out" $! show $! waysToAccept line g
