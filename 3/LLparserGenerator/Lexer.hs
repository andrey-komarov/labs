{-# LANGUAGE GeneralizedNewtypeDeriving #-}
module Lexer where

import Control.Monad.State
import Control.Monad.Error
import Control.Applicative
import Data.Char
import Debug.Trace

data TokenMode
    = MRoot | MRegex | MCode | MComment | MString | MType
    deriving (Eq, Show)

data Token
    = TokenName String
    | RuleName String
    | JavaCode String
    | Regex String
    | Comment String
    | Colon | Semicolon
    | ColonColon | Dot | Or | TokenEOF | Star
    deriving (Eq, Show)

type StateStack = [TokenMode]
type LexerState = (StateStack, String)

newtype P a = P (ErrorT String (State LexerState) a) deriving
    (Monad, MonadError String, MonadState LexerState, Functor, Applicative)

runP :: P a -> LexerState -> Either String a
runP (P p) = evalState (runErrorT p)

getMode :: P TokenMode
getMode = head <$> fst <$> get

pushMode :: TokenMode -> P ()
pushMode m = modify $ \(xm, s) -> (m:xm, s)

popMode :: P TokenMode
popMode = do
    (m:xm, s) <- get
    put (xm, s)
    return m

getS :: P String
getS = snd <$> get

putS :: String -> P ()
putS s = modify $ \(xm, _) -> (xm, s)

popWhile :: (Char -> Bool) -> P String
popWhile p = do
    (s, rest) <- span p <$> getS
    putS rest
    return s

-- It's ugly :(
getCode :: P String
getCode = do
    (mm@(m:xm), s) <- get
    case (m, s) of
        -- Close comment
        (MComment, '*':'/':xs) -> do
            c <- modify (const (filter (/= MComment) xm, xs)) >> getCode
            return $ "*/" ++ c
        (MComment, x:xs) -> do
            c <- putS xs >> getCode
            return $ x:c
        -- Escaped \" in string literal
        (MString, '\\':'"':xs) -> do
            c <- putS xs >> getCode
            return $ "\\\"" ++ c
        -- End of string literal
        (MString, '"':xs) -> do
            c <- popMode >> putS xs >> getCode
            return $ '"':c
        (MString, '\n':xs) -> 
            throwError $ "String literal is still unclosed in the EOLN: " ++ take 50 s        
        (MString, x:xs) -> do
            c <- putS xs >> getCode
            return $ x:c
        -- Open multi-line comment, we are not in MString
        (_, '/':'*':xs) -> do
            c <- putS xs >> pushMode MComment >> getCode
            return $ "/*" ++ c
        -- Open code block, we are not in MString or MComment
        (_, '{':xs) -> do
            c <- putS xs >> pushMode MCode >> getCode
            return $ '{':c
        -- Close code block
        (MCode, '}':xs) -> do
            c <- putS xs >> popMode >> getCode
            return $ '}':c
        -- We are done here
        (_, '}':xs) -> do
            putS xs
            return []
        (_, x:xs) -> do
            c <- putS xs >> getCode
            return $ x:c
        (_, _) -> 
            throwError $ "Something went wrong " ++ show mm ++ ", " ++ s


code :: P Token
code = return . JavaCode =<< getCode

comment :: P Token
comment = return . Comment =<< popWhile (/= '\n')

regex :: P Token
regex = return . Regex =<< popWhile (/= '\n')

type_ :: P Token
type_ = return . JavaCode =<< popWhile (/= '\n')

lexer :: (Token -> P a) -> P a
lexer cont = do
    s <- getS
    m <- getMode
    f m s >>= cont where
        f :: TokenMode -> String -> P Token
        f MRoot (x:xs) | isSpace x = putS xs >> f MRoot xs
        f MRoot ('{':xs) = putS xs >> code
        f MRoot ('#':xs) = comment
        f MRegex _ = popMode >> regex
        f MRoot (x:xs) | isAlpha x = do
            name <- popWhile isAlpha
            return $ (if isLower x then RuleName else TokenName) name
        f MRoot (':':':':xs) = putS xs >> pushMode MRegex >> return ColonColon
        f MRoot ('.':xs) = putS xs >> pushMode MType >> return Dot
        f MRoot (':':xs) = putS xs >> return Colon
        f MRoot (';':xs) = putS xs >> return Semicolon
        f MRoot ('|':xs) = putS xs >> return Or
        f MRoot ('*':xs) = putS xs >> return Star
        f MType s = popMode >> type_ 
        f _ [] = return TokenEOF
        f a b = trace (show a ++ " " ++ show b) undefined

lexP :: P [Token]
lexP = do
    a <- lexer return
    if a == TokenEOF 
        then return [a]
        else do
            rest <- lexP
            return $ a:rest
