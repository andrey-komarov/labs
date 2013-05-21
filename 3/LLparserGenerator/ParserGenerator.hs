module ParserGenerator where

import Datatypes
import Utils

import Control.Monad.Writer
import Control.Monad.State
import Control.Monad.Error
import Control.Applicative

import qualified Data.Map as M
import Data.Map((!))
import qualified Data.Set as S

import Data.Foldable (toList)

import Data.Char
import Data.List

generateParser :: String -> [Item] -> Gen ()
generateParser name items = do
    let cname = name ++ "Parser"
    let lexer = name ++ "Lexer"
    let lexem = name ++ "Lexem"
    let mainNode = clazzify $ head $ [r | RuleDecl r _ <- items]
    let head = [ "public class " ++ cname ++ "{"
               , "    static String expect(Lexer<" ++ lexem ++ "> lexer, " ++lexem++ ".Tag tag) {"
               , "        " ++ lexem ++ " lexem = lexer.peekLexem();"
               , "        if (lexem.tag != tag) {"
               , "            throw new ParserException(\"Unxepected token: \" + lexem.tag);"
               , "        }"
               , "        return lexer.nextLexem().value;"
               , "    }"
               ]
    nodes <- concat <$> sequence [generateClass lexem c items | RuleDecl c _ <- items]
    parsers <- concat <$> sequence [ generateParseRule lexem cl items | RuleDecl cl _ <- items]
    let tail = [ "    static public " ++ mainNode ++ " parse(String s) {"
               , "        " ++ lexer ++ " lexer = new " ++ lexer ++ "(s);"
               , "        " ++ mainNode ++ " res = parse" ++ mainNode ++ "(lexer);"
               , "        if (lexer.peekLexem() != " ++ lexem ++ ".EOF) {"
               , "            throw new ParserException(\"Not all input proceeded\");"
               , "        }"
               , "        return res;"
               , "    }"
               , "}"
               ]
    let res = head ++ map indent nodes ++ map indent parsers ++ tail
    tell ["", "# " ++ name, "" ]
    tell ["=============================" ++ show (length nodes)]
--    tell $ [head $ nodes]
--    tell $ [indent $ head $ nodes]
    modify $ M.insert (cname ++ ".java") $ unlines res
    return ()

capitalize :: String -> String
capitalize (c:xs) = toUpper c:xs

clazzify :: String -> String
clazzify = ("Node" ++) . capitalize

indent :: String -> String
indent = ("    " ++)

clazzifyMany :: String -> [String] -> String
clazzifyMany base rules = intercalate "_" rules ++ "_" ++ clazzify base

generateClass :: String -> String -> [Item] -> Gen [String]
generateClass lexem clazz items = do
    tell [ "#### Generating classes for `" ++ clazz ++ "`" ]
    let cname = clazzify clazz
    let s1 = [ "static abstract public class " ++ cname ++ " {"]
          ++ [ "    " ++ tp ++ " " ++ name ++ ";" | AttrDecl c name tp <- items, c == clazz]  
          ++ [ "}" ]
    let mkName r = clazzifyMany clazz (rule r)
    let arglist r = intercalate ", " 
         [ (if isToken ru then "String" else clazzify ru) ++ " _" ++ show n | 
            (ru, n) <- zip (rule r) [1..]]
    let cons r = [ "public " ++ mkName r ++ "(" ++ arglist r ++ ") {" ]
              ++ [ "    this._" ++ show n ++ " = _" ++ show n ++ ";" | 
                    (ru, n) <- zip (rule r) [1..], isRule ru]
              ++ [ indent code | WithCode _ code <- [r]]
              ++ [ "}" ]
    let subcl r = let iname = mkName r in
            [ "static public class " ++ iname ++ " extends " ++ cname ++ "{" ]
         ++ [ "    " ++ (clazzify ru) ++ " _" ++ show n ++ ";" | 
                (ru, n) <- zip (rule r) [1..], isRule ru]
         ++ map indent (cons r)
         ++ [ "}" ]
    let s2 = concat [ subcl r | RuleDecl a rr <- items, a == clazz, r <- rr]
    return $ s1 ++ s2

pairs :: [a] -> [(a, a)]
pairs []     = []
pairs (a:xs) = pairs xs ++ [(a, x) | x <- xs] ++ [(x, a) | x <- xs]

generateParseRule :: String -> String -> [Item] -> Gen [String]
generateParseRule lexem clazz items = do
    let cname = clazzify clazz
    let f = first items
    let flw = follow items
    let rr = head $ [map rule rr | RuleDecl q rr <- items, q == clazz]
    let fsts = [firstS f r | r <- rr] 
    let withEps = filter (eps `S.member`) fsts
    let bad = [(fa, fb) | (fa, fb) <- pairs fsts, not (S.null $ fa `S.intersection` fb) ]
    unless(null bad) $ do
        tell [ "* **Error**: in rule " ++ clazz ++ " FIRST sets " ++ 
                (ppx fa) ++ " and " ++ (ppx fb) ++ " intersect " | (fa, fb) <- bad]
        throwError $ "Intersecting FIRST sets"
    let bad = [(fa, fb) | (fa, fb) <- pairs fsts, eps `S.member` fb, 
                not (S.null $ (flw!clazz) `S.intersection` fa)]
    unless (null bad) $ do
        tell [ "* **Error**: " ++ ppx fb ++ " can produce EPS, but " ++ 
               "FIRST " ++ ppx fa ++ " and FOLLOW " ++ clazz ++ " intersect" 
             | (fa, fb) <- bad]
        throwError $ "interection of FOLLOW A and FIRST alpha is not empty"
    let res = [(a, [t]) | a <- rr, t <- toList (firstS f a), t /= eps]
           ++ [(a, [t]) | a <- rr, eps `S.member` firstS f a, t <- toList (flw!clazz), t /= eps]
    let s1 = [ "static private " ++ cname ++ " parse" ++ cname ++ "(Lexer<" ++ lexem ++ "> lexer) {"             , "    " ++ lexem ++ " lexem = lexer.peekLexem();"
             , "        switch (lexem.tag) {"             
             ]
    let mk1 (rule, tokens) = map (indent . indent . indent) [ unwords $ map 
              (\s -> "case " ++ s ++ ":") tokens]
         ++ ["        {"] ++ map (indent . indent . indent . indent) [ if isToken tp 
               then "String _" ++ show n ++ " = expect(lexer, " ++ lexem ++ ".Tag." ++ tp ++ ");"
               else (clazzify tp) ++ " _" ++ show n ++ " = parse" ++ clazzify tp ++ "(lexer);"
            | (n, tp) <- zip [1..] rule]
         ++ ["             return new " ++ clazzifyMany clazz rule ++ "(" 
             ++ intercalate "," (map (("_"++) . show) $ take (length rule) [1..]) 
             ++ ");"]
         ++ ["        }"]
    let s2 = concat [mk1 a | a <- M.toList (M.fromListWith (<>) res)]
    let s3 = map (indent . indent) [ "default: {"
             , "    throw new ParserException(\"Unexpected token in parseNodeE: \" + lexem);"
             , "}"
             ]
    let s4 = [ "    }"
             , "}"
             ]
    return $ s1 ++ s2 ++ s3 ++ s4
