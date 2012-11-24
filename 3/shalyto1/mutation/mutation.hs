import System.IO
import Control.Applicative

solve :: (String, String) -> Double
solve (s1, s2) = product $ zipWith mutate s1 s2 where 
    mutate = \c1 c2 -> if c1 == c2 then (1-p) else p
    p = 1 / (fromIntegral $ length s1)

toPairs :: [a] -> [(a, a)]
toPairs [] = []
toPairs (x:y:xs) = (x, y):toPairs xs

main = do
    input <- toPairs <$> tail <$> lines <$> readFile "mutation.in"
    writeFile "mutation.out" $ concatMap ((++"\n") . show . solve) input

