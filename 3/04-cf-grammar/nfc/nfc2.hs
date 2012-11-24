import System.IO
import qualified Data.Set as S
import qualified Data.Map as M
import Control.Applicative
import Data.List
import Data.Array.ST
import Control.Monad.ST
import Data.Int
import Data.Array as A

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
    | Finish {-# UNPACK #-} !Char deriving Show

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
    mgo = M.fromListWith (++) $! map (\(x, y) -> (x, [y])) rx 
    mgo' = array ('A', 'Z') $! M.toList mgo
    go a = mgo' ! a -- (M.findWithDefault [] a mgo)
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
        dp a 0 n (start g)

    dp :: STUArray s (Int, Int, Char) Int64 -> Int -> Int -> Char -> ST s Int64
    dp d left right rule = do
        let n = right - left
        val <- readArray d (left, right, rule)
        if val >= 0
            then return val
            else do
                let rulesFromMe = do
                                    Produce c1 c2 <- rules g rule
                                    return $! (c1, c2) 
                a <- sequence $! [
                    do 
                        n1 <- dp d left (left+i) c1
                        n2 <- dp d (left+i) right c2
                        return $! n1 * n2 `mod` modulo
                    | i <- [1..n-1], (c1,c2) <- rulesFromMe]
                let ans = sum a `mod` modulo
                writeArray d (left, right, rule) $! ans
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


