import Sadly
import Lexer
import Datatypes
import Utils
import LexerGenerators
import ParserGenerator
import Validators

import System.IO

import Control.Applicative
import Control.Monad.Writer
import Control.Monad.Error
import Control.Monad.State

import Data.List

import qualified Data.Map as M
import Data.Map (Map, (!))

import Debug.Trace

mkRules :: [Item] -> [GRule]
mkRules items = [(from, map rule to) | RuleDecl from to <- items]

parseConfig :: String -> Gen [Item]
parseConfig s =
    case runParser s of
        Left err -> throwError err
        Right r -> return r

implement :: String -> String -> Gen ()
implement name s = do
    tell ["# Generating " ++ name ++ " recognizer" ]
    r <- parseConfig s
    checkAll r
    generateLexer name r
    let f =  first r

    tell [ "", "# GRAMMAR " ]
    tell [ "* " ++ from ++ " : " ++ ppx rr | RuleDecl from to <- r, rr <- map rule to]
    
    tell [ "", "# FIRST " ]
    tell [ "* FIRST " ++ k ++ " = " ++ ppx (f!k) | k <- M.keys f , isRule k]
    let f =  follow r
    tell [ "", "# FOLLOW" ]
    tell [ "* FOLLOW " ++ k ++ " : " ++ ppx (f!k) | k <- M.keys f , isRule k]
    generateParser name r
    return ()

main = do
    s <- getContents
    let (e, log, files) = runGen $ implement "Sadly" s
    putStrLn $ unlines log
    case e of
        Left err ->
            putStrLn $ "# ERROR! " ++ err
        Right _ -> do
            putStrLn "# SUCCESS!"
            forM_ (M.toList files) $ \(name, content) -> do
                putStrLn $ "* Writing file " ++ name ++ ", length = " ++ show (length content)
                writeFile name content
            putStrLn "# Done!"
