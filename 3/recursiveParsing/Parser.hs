import Data.Char (isSpace, isLetter)
import Data.List

data Lexem 
    = Colon
    | Semicolon
    | Comma
    | Space
    | Name String
    | NoParse String deriving (Eq, Show, Read)

lexOne :: String -> Lexem
lexOne ":" = Colon
lexOne ";" = Semicolon
lexOne "," = Comma
lexOne s
    | all isLetter s = Name s
    | all isSpace s = Space
    | otherwise = NoParse $ "Unknown lexem: \"" ++ s ++ "\""

lexer :: String -> Either String [Lexem]
lexer s = if null bad then Right res else Left (unNoParse $ head bad) where 
    res = filter (/= Space) $ map lexOne $ groupBy (\a b -> isLetter a && isLetter b) s
    bad = filter isNoParse res
    isNoParse (NoParse _) = True
    isNoParse _ = False
    unNoParse (NoParse s) = s

type ParseError = String

data Parser a = Parser {runParser :: [Lexem] -> Either ParseError ([Lexem], a)}

instance Monad Parser where
    return a = Parser $ \s -> Right (s, a)
    ma >>= f = Parser $ \s -> case runParser ma s of
        Left e -> Left e
        Right (s', a) -> runParser (f a) s'

err = Left

expect :: Lexem -> Parser Lexem
expect a = Parser $ \s -> case s of
    [] -> err $ "Expected " ++ show a ++ ", EOLN found"
    (x:xs) -> if a == x
        then Right (xs, x)
        else err $ "Expected " ++ show a ++ ", " ++ show x ++ " found"

like :: (Lexem -> Bool) -> Parser Lexem
like p = Parser $ \s -> case s of
    [] -> err $ "Unexpected EOLN"
    (x:xs) -> if p x
        then Right (xs, x)
        else err $ "Unexpected " ++ show x

isEOLN :: Parser Bool
isEOLN = Parser $ \s -> Right $ (s, null s)

parseEOLN :: Parser ()
parseEOLN = Parser $ \s -> case s of
    [] -> Right ([], ())
    s -> err $ "Unexpected EOLN"

peek :: Parser (Maybe Lexem)
peek = Parser $ \s -> case s of
    [] -> Right (s, Nothing)
    s@(x:xs) -> Right (s, Just x)

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

isName :: Lexem -> Bool
isName (Name _) = True
isName _ = False

parseS :: Parser SNode
parseS = do
    expect $ Name "var"
    d <- parseD
    parseEOLN
    return $ SNode d

parseD :: Parser DNode
parseD = do
    next <- peek
    case next of
        Nothing -> return DEps
        Just _ -> do
            e <- parseE
            d <- parseD
            return $ DNode e d

parseE :: Parser ENode
parseE = do
    Name name <- like isName
    rest <- parseI
    expect Colon
    Name tp <- like isName
    expect Semicolon
    return $ ENode name rest tp

parseI :: Parser INode
parseI = do
    next <- peek
    case next of
        Just Comma -> do
            expect Comma
            Name name <- like isName
            rest <- parseI
            return $ INode name rest
        _ -> return IEps

parse :: String -> Either String Node
parse s = do
    lex <- lexer s
    t <- runParser parseS lex
    return $ snd t

main = do
--    let s = "var a : int; b , c :int;"
    let s = "var a,b,c,dd : int; ee : double;"
    print $ parse s
