import System.IO
import Control.Applicative
import Data.List

onePoint :: Eq a => [a] -> [a] -> [a] -> Bool
onePoint x y z = 
    (dropWhile (\(x,y,z) -> y == z) $
    dropWhile (\(x,y,z) -> x == z) $ zip3 x y z) == []

twoPoint :: Eq a => [a] -> [a] -> [a] -> Bool
twoPoint x y z = 
    (dropWhile (\(x,y,z) -> x == z) $
    dropWhile (\(x,y,z) -> y == z) $
    dropWhile (\(x,y,z) -> x == z) $ zip3 x y z) == []

monotone :: Eq a => [a] -> [a] -> [a] -> Bool
monotone x y z = all (\(x,y,z) -> x == z || y == z) $ zip3 x y z

yesno :: Bool -> String
yesno True = "YES"
yesno False = "NO"

main = do
    inf <- openFile "crossover.in" ReadMode
    out <- openFile "crossover.out" WriteMode
    [n, m] <- map read <$> words <$> hGetLine inf
    ss <- sequence $ take n $ repeat $ hGetLine inf
    s <- hGetLine inf
    hPutStrLn out $ intercalate "\n" $ map (yesno . any id . \f -> [f x y s | x <- ss, y <- ss])
                [onePoint, twoPoint, monotone]
    hClose out
