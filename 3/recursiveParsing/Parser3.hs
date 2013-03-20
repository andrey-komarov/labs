{-# LANGUAGE GADTs, GeneralizedNewtypeDeriving #-}
import Control.Monad.State
import Control.Monad.Error
import Control.Monad.Writer
import Data.Char
import Data.Monoid
import System.Cmd

newtype Parser a b = Parser {
    runP :: ErrorT String (State [a]) b
} deriving (Monad, MonadError String, MonadState [a])

runParser :: Parser s a -> [s] -> Either String a
runParser (Parser p) = (fmap fst . runState . runErrorT) p

data C a = C a | EOLN deriving (Eq, Ord, Show, Read)

peek1 :: Parser a (C a)
peek1 = liftM peek1' get where
    peek1' [] = EOLN
    peek1' (x:_) = C x

like :: Show a => (a -> Bool) -> String -> Parser a a
like p desc = do
    s <- get
    when (null s) $ throwError ("Expected " ++ desc ++ ", found EOLN") >> return ()
    let x = head s
    if p x
        then put (tail s) >> return x
        else throwError $ "Expected " ++ desc ++ ", found " ++ show x

expect1 :: (Eq a, Show a) => a -> Parser a a
expect1 a = like (== a) (show a)

expect :: (Eq a, Show a) => [a] -> Parser a a
expect a = like (`elem` a) ("one of " ++ show a)

data Lexem 
    = Colon
    | Semicolon
    | Comma
    | Name String deriving (Eq, Show, Read)

isName :: Lexem -> Bool
isName (Name _) = True
isName _ = False

parseName :: Parser Char Lexem
parseName = do
    c <- like isLetter "letter"
    rest <- parseName2
    return $ Name $ c:rest

parseName2 :: Parser Char String
parseName2 = do
    c <- peek1
    case c of
        EOLN -> return []
        C c' -> if isAlpha c'
                    then do
                        like isAlpha "letter"
                        s <- parseName2
                        return $ c':s
                    else return []

tokSel :: Char -> Parser Char [Lexem]
tokSel ':' = expect1 ':' >> return [Colon]
tokSel ';' = expect1 ';' >> return [Semicolon]
tokSel ',' = expect1 ',' >> return [Comma]
tokSel c 
    | isSpace c = like isSpace "space" >> return []
    | isAlpha c = sequence [parseName]
    | otherwise = throwError ("Unknown token " ++ show c)

lexer :: Parser Char [Lexem]
lexer = do
    c <- peek1
    case c of
        EOLN -> return []
        C c -> tokSel c >>= \x -> liftM (x ++) lexer

{- S -> "var" D
 - D -> ED | eps
 - E -> n I : n ;
 - I -> , n I | eps
 -}

type Node = SNode
data SNode = SNode DNode
data DNode = DNode ENode DNode | DEps
data ENode = ENode String INode String
data INode = INode String INode | IEps

newtype Traverser a = Traverser {
    runT :: WriterT [String] (State Int) a
} deriving (Monad, MonadWriter [String], MonadState Int)

newId :: Traverser Int
newId = do
    n <- get
    put $ n + 1
    return n

class GViz a where
    gviz :: a -> Traverser Int

viz :: SNode -> String
viz t = let ((a, w), s) = (runState . runWriterT) (runT $ gviz t) 0
        in concatMap (++"\n") w

sameRank :: [Int] -> Traverser ()
sameRank xs = tell ["  { rank=\"same\"; " 
                ++ concatMap (\i -> "n" ++ show i ++ " ") xs
                ++ "}"]

link :: Int -> Int -> Traverser ()
link n m = tell ["  n" ++ show n ++ " -- n" ++ show m ++ ";"]

mkNode :: String -> String -> Traverser Int
mkNode label color = do
    n <- newId
    tell ["  n" ++ show n ++ " [label=\"" ++ label ++ "\"" ++ "color=\"" ++ color ++ "\"]"]
    return n

instance GViz SNode where
    gviz (SNode d) = do
        tell ["graph {"]
        n <- mkNode "S" "red"
        m <- mkNode "Var" "blue"
        n2 <- gviz d
        sameRank [m, n2]
        link n m
        link n n2
        tell ["}"]
        return n

instance GViz DNode where
    -- D -> ED | eps
    gviz (DNode e d) = do
        n <- mkNode "D" "red"
        e' <- gviz e
        d' <- gviz d
        sameRank [e', d']
        link n e'
        link n d'
        return n
    gviz DEps = mkNode "D" "red"

instance GViz ENode where
    -- E -> n I : n ;
    gviz (ENode name1 i name2) = do
        n  <- mkNode "E" "red"
        n1 <- mkNode ("ID " ++ name1) "black"
        n2 <- gviz i
        n3 <- mkNode ":" "green"
        n4 <- mkNode ("ID " ++ name2) "orange"
        n5 <- mkNode ";" "green"
        sameRank [n1, n2, n3, n4, n5]
        link n n1
        link n n2
        link n n3
        link n n4
        link n n5
        return n

instance GViz INode where
    -- I -> , n I | eps
    gviz (INode name i) = do
        n <- mkNode "I" "red"
        n1 <- mkNode "," "green"
        n2 <- mkNode ("ID " ++ name) "black"
        n3 <- gviz i
        sameRank [n1, n2, n3]
        link n n1
        link n n2
        link n n3
        return n
    gviz IEps = mkNode "I" "red"

instance Show SNode where
    show (SNode d) = "var\n" ++ show d

instance Show DNode where
    show (DNode e d) = show e ++ show d
    show DEps = ""

instance Show ENode where
    show (ENode name names tp) = "    " ++ name ++ show names ++ " : " ++ tp ++ ";\n"

instance Show INode where
    show (INode name names) = ", " ++ name ++ show names
    show IEps = ""

parseS :: Parser Lexem SNode
parseS = do
    expect1 $ Name "var"
    d <- parseD
    next <- peek1
    when (next /= EOLN) $ throwError $ "Expected EOLN, " ++ show next ++ " found"
    return $ SNode d

parseD :: Parser Lexem DNode
parseD = do
    next <- peek1
    case next of
        EOLN -> return DEps
        (C _) -> do
            e <- parseE
            d <- parseD
            return $ DNode e d

parseE :: Parser Lexem ENode
parseE = do
    Name name <- like isName "Variable name"
    rest <- parseI
    expect1 Colon
    Name tp <- like isName "Type name"
    expect1 Semicolon
    return $ ENode name rest tp

parseI :: Parser Lexem INode
parseI = do
    next <- peek1
    case next of
        C Comma -> do
            expect1 Comma
            Name name <- like isName "Variable name"
            rest <- parseI
            return $ INode name rest
        _ -> return IEps

parser = parseS

parse :: String -> Either String SNode
parse s = do
    lexems <- runParser lexer s
    runParser parser lexems

checkIO :: [(String, Bool)] -> IO ()
checkIO tests = do
    let (ok, log) = runWriter (check tests)
    forM log putStrLn
    putStrLn $ if ok then "OK" else "FAIL"

check :: [(String, Bool)] -> Writer [String] Bool
check [] = return True
check ((test, outcome):xs) = do
    tell $ ["================================================================================", show test]
    let p = parse test
    case p of
        Left err -> do
            tell ["...No parse: " ++ err]
            rest <- check xs
            return $ rest && not outcome
        Right s -> do
            tell [show s]
            rest <- check xs
            return $ rest && outcome

tests = let {y = True; n = False} in
    [ ("var a : int;", y)
    , ("vbr b : int;", n)
    , ("var a, b : int;", y)
    , ("var a, b :: int;", n)
    , ("var a, b : int; c : int;", y)
    , ("var a, b : int; c, d : int;", y)
    , ("var a,, b : int;", n)
    , ("var a, b, c : int;;", n)
    , ("var a : int", n)
    , ("v a r a : int", n)
    , ("     var a : int", n)
    , ("var", y)
    , ("var ololo : a;", y)
    , ("var ololo, tata, asda, asdf : asldfa; asdf, asdfas,f,asdf,asdf : double;a,b,c,d,e,f,g:bool;", y)
--    , ("var " ++ (concat $ take 10000 $ repeat " a : int; "), y)
    ]

unRight :: Either a b -> b
unRight (Right b) = b

runTestIO :: String -> IO ()
runTestIO test = do
    case parse test of
        Left err -> putStrLn $ "Parse error: " ++ err
        Right t -> do
            putStrLn "Parse successful"
            writeFile "tmp.dot" $ viz t
            system $ "dot -Tpng tmp.dot > tmp.png"
            system $ "feh tmp.png"
            return ()

loopIO :: IO ()
loopIO = do
    putStrLn "Enter Pascal type (or exit):"
    line <- getLine
    if line == "exit"
        then putStrLn "Bye!"
        else runTestIO line >> loopIO

main = do
    loopIO
--    checkIO tests
--    putStrLn $ viz $ unRight $ parse "var a, b, c, d : int; asdfa, asda, asd : ololo; d, e, f, g, h : double;"
