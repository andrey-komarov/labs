{-# LANGUAGE GADTs, GeneralizedNewtypeDeriving #-}
import Control.Monad.State
import Control.Monad.Error
import Control.Monad.Writer
import Data.Char
import Data.Monoid

data ParseError = ParseError String deriving (Show)
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
data SNode = SNode DNode deriving (Show)
data DNode = DNode ENode DNode | DEps deriving (Show)
data ENode = ENode String INode String deriving (Show)
data INode = INode String INode | IEps deriving (Show)

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
    putStrLn $ if ok then "FAIL" else "OK"

check :: [(String, Bool)] -> Writer [String] Bool
check [] = return True
check ((test, outcome):xs) = do
    tell $ ["Checking on test " ++ show test]
    let p = parse test
    case p of
        Left err -> do
            tell ["No parse: " ++ err]
            rest <- check xs
            return $ rest && not outcome
        Right s -> do
            tell ["Success!: " ++ show s]
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
    , ("     var a : int", y)
    ]

main = do
    checkIO tests
