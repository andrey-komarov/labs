module LexerGenerators (generateLexer) where

import Datatypes
import Utils

import Data.List
import Data.Char

import Control.Applicative
import Control.Monad.Writer
import Control.Monad.Error
import Control.Monad.State

import qualified Data.Set as S
import Data.Set (Set)
import qualified Data.Map as M
import Data.Map (Map)

generateLexer :: String -> [Item] -> Gen ()
generateLexer name items = do
    tell [ "## Generating lexer"
         ]
    generateException "LexerRecognizeException"
    generateException "ParserException"
    generateTokenClass name items
    generateLexerInterface
    generateLexerImplementation name items

generateLexerInterface :: Gen ()
generateLexerInterface = do
    let s = [ "public interface Lexer<Lexem> {"
            , "    Lexem nextLexem();"
            , "    Lexem peekLexem();"
            , "}"
            ]
    tell [ "### Generating lexer interface"
         ] 
    modify $ M.insert "Lexer.java" $ unlines s


generateLexerImplementation :: String -> [Item] -> Gen ()
generateLexerImplementation name items = do
    let fname = name ++ "Lexer"
    let lexfname = name ++ "Lexem"
    let s1 =   [ "import java.util.regex.*;"
               , "public class " ++ fname ++ " implements Lexer<" ++ lexfname ++ "> {"
               , "    private String s;"
               , "    private int start;"
               , "    private " ++ lexfname ++ " currentLexem = null;"
               ]
    let noSkip = [(t, r) | TokenDecl t r True  <- items]
    let skip   = [(t, r) | TokenDecl t r False <- items]
    let s2 =   [ "    private final int LEXEMS = " ++ show (length noSkip) ++ ";"
               , "    private final " ++ lexfname ++ ".Tag[] LEXEME_TYPE ="
               , "    { " ++ lexfname ++ ".Tag." ++ fst (head noSkip)
               ]
    let stail xs = if null xs then [] else tail xs
    let s3 =   map ((\t -> "    , " ++ lexfname ++ ".Tag." ++ t) . fst) $ stail noSkip
    let ltrim = dropWhile isSpace
    let s4 =   [ "    };"
               , "    private final Pattern[] PATTERNS ="
               , "    { Pattern.compile(\"^" ++ ltrim (snd (head noSkip)) ++ "\")"
               ]
    let s5 =   map ((\t -> "    , Pattern.compile(\"^" ++ ltrim t ++ "\")") . snd) $ stail noSkip
    let s6 =   [ "    };"
               , ""
               , "    private final Matcher[] MATCHERS;"
               , ""
               , "    private final int SKIP_LEXEMS = " ++ show (length skip) ++ ";"
               , "    private final Pattern[] SKIP_PATTERNS ="
               , if null skip then "{" else "            { Pattern.compile(\"^" ++ ltrim (snd (head skip)) ++ "\")"
               ]
    let s7 =   map ((\t -> "    , Pattern.compile(\"^" ++ ltrim t ++ "\")") . snd) $ stail skip
    let s8 =   [ "    };"]
    let s9 =   [ "    private final Matcher[] SKIP_MATCHERS;"
               , "    " ++ fname ++ "(String s) {"
               , "        this.s = s;"
               , "        start = 0;"
               , "        MATCHERS = new Matcher[LEXEMS];"
               , "        for (int i = 0; i < LEXEMS; i++) {"
               , "            MATCHERS[i] = PATTERNS[i].matcher(s);"
               , "        }"
               , "        SKIP_MATCHERS = new Matcher[SKIP_LEXEMS];"
               , "        for (int i = 0; i < SKIP_LEXEMS; i++) {"
               , "            SKIP_MATCHERS[i] = SKIP_PATTERNS[i].matcher(s);"
               , "        }"
               , "        skipAllSkipable();"
               , "    }"
               , ""
               , "    private void skipAllSkipable() {"
               , "        boolean changes;"
               , "        do {"
               , "            changes = false;"
               , "            for (Matcher m : SKIP_MATCHERS) {"
               , "                m.region(start, s.length());"
               , "                if (m.find()) {"
               , "                    start = m.end();"
               , "                    changes = true;"
               , "                    break;"
               , "                }"
               , "            }"
               , "        } while (changes);"
               , "    }"
               , ""
               , ""
               , "    @Override"
               , "    public " ++ lexfname ++ " nextLexem() {"
               , "        " ++ lexfname ++ " res = peekLexem();"
               , "        currentLexem = null;"
               , "        return res;"
               , "    }"
               , ""
               , "    @Override"
               , "    public " ++ lexfname ++ " peekLexem() {"
               , "        if (currentLexem != null) {"
               , "            return currentLexem;"
               , "        }"
               , "        for (int i = 0; i < LEXEMS; i++) {"
               , "            MATCHERS[i].region(start, s.length());"
               , "            if (MATCHERS[i].find()) {"
               , "                " ++ lexfname ++ " result = new " ++ lexfname ++ "(LEXEME_TYPE[i], MATCHERS[i].group());"
               , "                start = MATCHERS[i].end();"
               , "                skipAllSkipable();"
               , "                return currentLexem = result;"
               , "            }"
               , "        }"
               , "        if (start != s.length()) {"
               , "            throw new LexerRecognizeException(s.substring(start, Math.min(start + 10, s.length())));"
               , "        } else {"
               , "            return currentLexem = " ++ lexfname ++ ".EOF;"
               , "        }"
               , "    }"
               , "}"
               ] 
    tell [ "### Generating lexer implementation"
         , "* Tokens:"
         ]
    tell [ "    + " ++ pp t ++ " : `" ++ r ++ "`" | (t, r) <- noSkip ]
    tell [ "* Tokens to skip:"]
    tell [ "    + " ++ pp t ++ " : `" ++ r ++ "`" | (t, r) <- skip ]
    let s = s1 ++ s2 ++ s3 ++ s4 ++ s5 ++ s6 ++ s7 ++ s8 ++ s9
    modify $ M.insert (fname ++ ".java") $ unlines s


generateException :: String -> Gen ()
generateException ex = do
    let s = [ "public class " ++ ex ++ " extends RuntimeException {"
            , "    public " ++ ex ++ "() {"
            , "    }"
            , ""
            , "    public " ++ ex ++ "(String message) {"
            , "        super(message);"
            , "    }"
            , ""
            , "    public " ++ ex ++ "(String message, Throwable cause) {"
            , "        super(message, cause);"
            , "    }"
            , ""
            , "    public " ++ ex ++ "(Throwable cause) {"
            , "        super(cause);"
            , "    }"
            , ""
            , "    public " ++ ex ++ "(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {"
            , "        super(message, cause, enableSuppression, writableStackTrace);"
            , "    }"
            , "}"
            ]

    tell [ "### Generating " ++ ex ]
    modify $ M.insert (ex ++ ".java") $ unlines s


generateTokenClass :: String -> [Item] -> Gen ()
generateTokenClass name items = do
    let fname = name ++ "Lexem"
    let noSkip = [(t, r) | TokenDecl t r True  <- items]
    let s1 = [ "public class " ++ fname ++ " {"
             , "    public enum Tag {"
             ]
    let s2 = [ "        " ++ intercalate ", " (map fst noSkip) ++ ", EOF;"
             ]
    let s3 = [ "    }"
             , ""
             , "    public final Tag tag;"
             , "    public final String value;"
             , ""
             , "    public " ++ fname ++ "(Tag tag, String value) {"
             , "        this.tag = tag;"
             , "        this.value = value;"
             , "    }"
             , ""
             , "    @Override"
             , "    public String toString() {"
             , "        return tag + \" \" + value;"
             , "    }"
             , ""
             , "    public static " ++ fname ++ " EOF = new " ++ fname ++ "(Tag.EOF, \"\");"
             , "}"
             ]
    let s = s1 ++ s2 ++ s3
    tell [ "### Generating " ++ fname ++ " class"]
    modify $ M.insert (fname ++ ".java") $ unlines s

