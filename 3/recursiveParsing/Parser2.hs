import Prelude hiding (null, drop)
import Data.Char (isSpace, isLetter, isAlpha)
import Data.List hiding (drop)

data ParseError = ParseError String deriving (Show)
data Parser s a = Parser {runParser :: s -> Either ParseError (s, a)}

instance Monad (Parser s) where
    return a = Parser $ \s -> Right (s, a)
    ma >>= f = Parser $ \s -> case runParser ma s of
        Left e -> Left e
        Right (s', a) -> runParser (f a) s'

class Category cat where
    id :: cat a a
    (<.>) :: cat b c -> cat a b -> cat a c

instance Category Parser where
    id = Parser $ \s -> Right (s, s)
    pbc <.> pab = Parser $ \a -> case runParser pab a of
        Left err -> Left err
        Right (a', b) -> case runParser pbc b of
            Left err' -> Left err'
            Right (b', c) -> Right (a', c)

mkLL1Parser :: (a -> c) -> (c -> Parser a b) -> Parser a b
mkLL1Parser peek selector = Parser $ \a -> runParser (selector $ peek a) a

data C a = EOLN | C a

peek :: Parser [a] (C a)
peek = Parser $ \s -> case s of
    [] -> Right ([], EOLN)
    s@(x:xs) -> Right $ (s, C x)

drop :: Parser [a] ()
drop = Parser $ \s -> case s of
    [] -> Left $ ParseError "Dropping from empty list"
    (x:xs) -> Right (xs, ())

like :: (a -> Bool) -> Parser [a] a
like p = Parser $ \s -> case s of
    [] -> Left $ ParseError $ "like from empty list"
    (x:xs) -> if p x then Right $ (xs, x)
                     else Left $ ParseError "Predicate not matched"

expect :: Eq a => a -> Parser [a] a
expect a = like (== a)

parseFail :: String -> Parser a b
parseFail s = Parser $ \_ -> Left $ ParseError s

data Lexem 
    = Colon
    | Semicolon
    | Comma
    | End
    | Name String deriving (Eq, Show, Read)

parseName :: Parser String Lexem
parseName = do
    c <- like isLetter
    rest <- parseName2
    return $ Name $ c:rest

parseName2 :: Parser String String
parseName2 = do
    c <- peek
    case c of
        EOLN -> return []
        C c' -> if isAlpha c'
                    then do
                        drop
                        s <- parseName2
                        return $ c':s
                    else return []

peek' :: [a] -> C a
peek' [] = EOLN
peek' (x:_) = C x

lexer :: Parser String [Lexem]
lexer = mkLL1Parser peek' selector where
    selector (C ':') = do
        expect ':' 
        rest <- lexer
        return $ Colon:rest
    selector (C ';') = do
        expect ';' 
        rest <- lexer 
        return $ Semicolon:rest
    selector (C ',') = do
        expect ',' 
        rest <- lexer
        return $ Comma:rest
    selector EOLN = return []
    selector (C c)
        | isSpace c = like (\_ -> True) >> lexer
        | isAlpha c = do
            n <- parseName
            rest <- lexer
            return $ n:rest
        | otherwise = parseFail $ "Unexpected token " ++ show c
    
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

parseS :: Parser [Lexem] SNode
parseS = do
    expect $ Name "var"
    d <- parseD
    EOLN <- peek
    return $ SNode d

parseD :: Parser [Lexem] DNode
parseD = mkLL1Parser peek' selector where
    selector EOLN = return DEps
    selector (C _) = do
                        e <- parseE
                        d <- parseD
                        return $ DNode e d

parseE :: Parser [Lexem] ENode
parseE = do
    Name name <- like isName
    rest <- parseI
    expect Colon
    Name tp <- like isName
    expect Semicolon
    return $ ENode name rest tp

parseI :: Parser [Lexem] INode
parseI = mkLL1Parser peek' selector where
    selector (C Comma) = do
        expect Comma
        Name name <- like isName
        rest <- parseI
        return $ INode name rest
    selector _ = return IEps

parser = parseS

main = do
--    let s = "var a,b,c,dd : int; ee : double;"
    let s = "var " ++ concat (repeat "i : int ")
    print $ runParser (parser <.> lexer) s
