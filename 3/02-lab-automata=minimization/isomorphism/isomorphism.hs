import System.IO
import Data.Array.ST
import Control.Monad.ST
import Data.Array
import qualified Data.Set as S
import qualified Data.Map as M
import Control.Applicative
import Data.Monoid
--import Data.ByteString.Char8 as B

type Vertex = Int
type Turn = Char -> Maybe Vertex 
data DFA = DFA { startVertex :: Vertex
               , go :: Array Vertex (M.Map Char Vertex)
               , terminals :: S.Set Vertex
               }

getTokens :: Handle -> IO [String]
getTokens h = words <$> hGetLine h

parseTokens :: (Read a) => Handle -> IO [a]
parseTokens h = map read <$> getTokens h

getLines :: Handle -> Int -> IO [String]
getLines _ 0 = return []
getLines h n = do
    s <- hGetLine h
    ss <- getLines h $ n - 1
    return $ s:ss

addTransition :: STArray s Int (M.Map Char Vertex) -> (Vertex, Vertex, Char) 
                -> ST s () 
addTransition arr (v1, v2, ch) = do
    m <- readArray arr v1
    writeArray arr v1 $ M.insert ch v2 m

buildTransitionTable :: Int -> [(Vertex, Vertex, Char)] 
                        -> Array Int (M.Map Char Int)
buildTransitionTable n edges = runSTArray $ do
    a <- newArray (0, n - 1) M.empty
    mapM (addTransition a) edges
    return a 

getDFA :: Handle -> IO DFA 
getDFA h = do
    [n, m, k] <- parseTokens h
    terminals <- S.fromList <$> map (+ (-1)) <$> parseTokens h
    edges <- let parseTri = (\[a,b,c] -> (read a - 1, read b - 1, head c)) . words
             in map parseTri <$> getLines h m 
    let e = buildTransitionTable n edges
    return $ DFA 0 e terminals

instance Eq DFA where
    dfa1 == dfa2 = runST $ run where  
        setTrue :: Int -> STArray s Int Bool -> ST s ()
        setTrue i a = writeArray a i True

        run :: ST s Bool 
        run = do
            s1 <- newArray (bounds $ go dfa1) False
            s2 <- newArray (bounds $ go dfa2) False
            dfs s1 s2 (startVertex dfa1, startVertex dfa2)

        zipM :: Eq a => M.Map a b -> M.Map a c -> Maybe [(b,c)]
        zipM m1 m2 
            | M.keys m1 == M.keys m2 = Just $ zip (M.elems m1) (M.elems m2)
            | otherwise = Nothing

        dfs :: STArray s Int Bool -> STArray s Int Bool
                                            -> (Vertex, Vertex) -> ST s Bool
        dfs s1 s2 (v1, v2) = do
            u1 <- readArray s1 v1
            u2 <- readArray s2 v2
            case (u1, u2) of
                (True, True) -> return True
                (False, True) -> return False
                (True, False) -> return False
                (False, False) -> do
                    writeArray s1 v1 True
                    writeArray s2 v2 True
                    let m1 = (go dfa1) ! v1
                    let m2 = (go dfa2) ! v2
                    case zipM m1 m2 of
                        Nothing -> return False
                        Just pairs -> do
                            new <- mapM (dfs s1 s2) pairs
                            return $ all id new 

main = do
    input <- openFile "isomorphism.in" ReadMode
    dfa1 <- getDFA input
    dfa2 <- getDFA input
    out <- openFile "isomorphism.out" WriteMode
    hPutStrLn out $ if dfa1 == dfa2 then "YES" else "NO"
    hClose out
         
