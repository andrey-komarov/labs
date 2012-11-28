import Prelude hiding (Either(..))
import Control.Monad (mapM_)

data Direction = Left | Right | Stay

type State = Int
type Letter = Char
type TMFunction = State -> Letter -> (State, Letter, Direction)
data TM = TM
  { delta :: TMFunction,
    start :: State,
    endAC :: State,
    endRJ :: State,
    blank :: Letter }

data Snapshot = Snapshot
  { left  :: String,
    right :: String,
    state :: State }

instance Show Snapshot where
  show sn = show (state sn) ++ ", " ++ extractString sn

runTMIO :: TM -> String -> IO ()
runTMIO tm input = let sn = Snapshot { left = [], right = input, state = start tm } in
  do loopTMIO tm sn

loopTMIO :: TM -> Snapshot -> IO ()
loopTMIO tm sn = case state sn == endAC tm of
  True -> print sn >> putStrLn "accepted"
  False -> case state sn == endRJ tm of
    True -> print sn >> putStrLn "rejected"
    False -> print sn >> loopTMIO tm (stepTM tm sn)

runTM :: TM -> String -> (Bool, String)
runTM tm input = loopTM tm (Snapshot { left = [], right = input, state = start tm })

loopTM :: TM -> Snapshot -> (Bool, String)
loopTM tm sn = loophole tm sn (state sn)

extractString :: Snapshot -> String
extractString sn = reverse (left sn) ++ "|" ++ right sn

loophole :: TM -> Snapshot -> State -> (Bool, String)
loophole tm sn c
  | c == endAC tm = (True, extractString sn)
  | c == endRJ tm = (False, extractString sn)
  | otherwise     = loopTM tm (stepTM tm sn)

goLeft :: TM -> Snapshot -> Snapshot
goLeft tm sn = case left sn of
  [] -> sn { right = blank tm : right sn }
  (x:xs) -> sn { left = xs, right = x : right sn }

goRight :: TM -> Snapshot -> Snapshot
goRight tm sn = case right sn of
  [x] -> sn { left = x : left sn, right = [blank tm] }
  (x:xs) -> sn { left = x : left sn, right = xs }

stepTM :: TM -> Snapshot -> Snapshot
stepTM tm sn = case right sn of
  (x:xs) -> case delta tm (state sn) x of
    (st', x', dir) -> let midSn = sn { state = st', right = x':xs } in
      case dir of
        Left  -> goLeft tm midSn
        Right -> goRight tm midSn
        Stay  -> midSn

runTests :: TM -> [String] -> IO ()
runTests tm tests = mapM_ (runTMIO tm) tests

zeros :: TM
zeros = TM
    { delta = \s l -> case (s, l) of
                    (0, '1') -> (98, '0', Right)
                    (0, '0') -> (0, ' ', Right)
                    (0, ' ') -> (99, ' ', Stay)
    , start = 0
    , endAC = 99
    , endRJ = 98
    , blank = ' '
    }

inc :: TM
inc = TM
    { delta = \s l -> case (s, l) of
                    (-1, ' ') -> (0, ' ', Left)
                    (-1, x) -> (-1, x, Right)
                    (0, '1') -> (0, '0', Left)
                    (0, '0') -> (99, '1', Stay)
                    (0, ' ') -> (99, '1', Stay)
    , start = -1
    , endAC = 99
    , endRJ = 98
    , blank = ' '
    }

palin :: TM
palin = TM
    { delta = \s l -> case (s, l) of
                        (0, ' ') -> (99, ' ', Stay)
                        (0, '0') -> (1, ' ', Right)
                        (0, '1') -> (-1, ' ', Right)
                        (1, ' ') -> (2, ' ', Left)
                        (1, x) -> (1, x, Right)
                        (2, '1') -> (98, '1', Stay)
                        (2, ' ') -> (99, ' ', Stay)
                        (2, '0') -> (3, ' ', Left)
                        (3, ' ') -> (0, ' ', Right)
                        (3, x) -> (3, x, Left)
                        (-1, ' ') -> (-2, ' ', Left)
                        (-1, x) -> (-1, x, Right)
                        (-2, '0') -> (98, '0', Stay)
                        (-2, ' ') -> (99, ' ', Stay)
                        (-2, '1') -> (3, ' ', Left)
    , start = 0
    , endAC = 99
    , endRJ = 98
    , blank = ' '
    }

tandemRepeat :: TM
tandemRepeat = TM
    { delta = \s l -> case (s, l) of
                        (0, '0') -> (1, 'a', Stay)
                        (0, '1') -> (1, 'b', Stay)
                        (1, ' ') -> (2, ' ', Left)
                        (1, 'A') -> (2, 'A', Left)
                        (1, 'B') -> (2, 'B', Left)
                        (1, x) -> (1, x, Right)
                        (2, '0') -> (3, 'A', Left)
                        (2, '1') -> (3, 'B', Left)
                        (3, '0') -> (3, '0', Left)
                        (3, '1') -> (3, '1', Left)
                        (3, 'a') -> (0, 'a', Right)
                        (3, 'b') -> (0, 'b', Right)
                        
                        (0, 'A') -> (4, 'A', Stay)
                        (0, 'B') -> (4, 'B', Stay)
                        (4, ' ') -> (5, ' ', Right)
                        (4, x) -> (4, x, Left)
                        
                        (5, 'a') -> (6, 'x', Right)
                        (5, 'b') -> (8, 'x', Right)
                        (5, 'X') -> (99, 'X', Stay)
                        (6, 'a') -> (6, 'a', Right)
                        (6, 'b') -> (6, 'b', Right)
                        (6, 'A') -> (7, 'X', Left)
                        (6, 'B') -> (98, 'B', Stay)
                        (6, 'X') -> (6, 'X', Right)
                        (7, 'x') -> (5, 'x', Right)
                        (7, x) -> (7, x, Left)
                        (8, 'a') -> (8, 'a', Right)
                        (8, 'b') -> (8, 'b', Right)
                        (8, 'A') -> (98, 'A', Stay)
                        (8, 'B') -> (7, 'X', Left)
                        (8, 'X') -> (8, 'X', Right)
                        (_, _) -> (98, '!', Stay)
    , start = 0
    , endAC = 99
    , endRJ = 98
    , blank = ' '
    }

cmp :: TM
cmp = TM
    { delta = \s l -> case (s, l) of
                        (0, '0') -> (1, 'a', Right)
                        (0, '1') -> (1, 'b', Right)
                        (1, ' ') -> (2, ' ', Left)
                        (1, 'A') -> (2, 'A', Left)
                        (1, 'B') -> (2, 'B', Left)
                        (1, x) -> (1, x, Right)
                        (2, '0') -> (3, 'A', Left)
                        (2, '1') -> (3, 'B', Left)
                        (3, 'a') -> (0, 'a', Right)
                        (3, 'b') -> (0, 'b', Right)
                        (3, x) -> (3, x, Left)

                        (2, '<') -> (98, '<', Stay)
                        (0, '<') -> (4, '<', Right)
                        (4, '1') -> (99, '1', Stay)
                        (4, 'A') -> (5, 'A', Left)
                        (4, 'B') -> (5, 'B', Left)
                        (5, x) -> (6, x, Left)
                        (6, 'a') -> (6, 'a', Left)
                        (6, 'b') -> (6, 'b', Left)
                        (6, ' ') -> (7, ' ', Right)
                        
                        (7, 'a') -> (8, 'x', Right)
                        (8, 'a') -> (8, 'a', Right)
                        (8, 'b') -> (8, 'b', Right)
                        (8, 'X') -> (8, 'X', Right)
                        (8, '<') -> (8, '<', Right)
                        (8, 'B') -> (99, 'B', Stay)
                        (8, 'A') -> (9, 'X', Left)
                        (9, 'a') -> (9, 'a', Left)
                        (9, 'b') -> (9, 'b', Left)
                        (9, '<') -> (9, '<', Left)
                        (9, 'X') -> (9, 'X', Left)
                        (9, 'x') -> (7, 'x', Right)

                        (7, 'b') -> (11, 'x', Right)
                        (11, 'a') -> (11, 'a', Right)
                        (11, 'b') -> (11, 'b', Right)
                        (11, 'X') -> (11, 'X', Right)
                        (11, '<') -> (11, '<', Right)
                        (11, 'A') -> (98, 'A', Stay)
                        (11, 'B') -> (12, 'X', Left)
                        (12, 'a') -> (12, 'a', Left)
                        (12, 'b') -> (12, 'b', Left)
                        (12, 'X') -> (12, 'X', Left)
                        (12, '<') -> (12, '<', Left)
                        (12, 'x') -> (7, 'x', Right)
                        
                        (7, '<') -> (98, '<', Stay)

    , start = 0
    , endAC = 99
    , endRJ = 98
    , blank = ' '
    }

