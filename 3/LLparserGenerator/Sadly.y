{
module Sadly where

import Lexer
import Datatypes
import Control.Monad.Error
}

%name sadly
%tokentype { Token }

%monad { P }
%lexer { lexer } { TokenEOF }

%token
    '::'          { ColonColon }
    ':'           { Colon }
    ';'           { Semicolon }
    '.'           { Dot }
    '|'           { Or }
    '*'           { Star }
    token         { TokenName $$ }
    rule          { RuleName $$ }
    code          { JavaCode $$ }
    regex         { Regex $$ }
    comment       { Comment $$ }
    
%%

Items : Item Items { $1:$2 }
      |            { [] }

Item : TokenDecl   { $1 }
     | RuleDecl    { $1 }
     | AttrDecl    { $1 }
     | Comment     { Com $1 }

TokenDecl : token '::' regex     { TokenDecl $1 $3 True }
          | '*' token '::' regex { TokenDecl $2 $4 False }

RuleDecl : rule ':' Subrules ';' { RuleDecl $1 $3 }

AttrDecl : rule ':' rule '.' code { AttrDecl $1 $3 $5 }

Comment : comment  { $1 }

Subrules : Rule                { [$1]  }
         | Rule '|' Subrules   { $1:$3 }

Rule : List         { WithoutCode $1 }
     | List code    { WithCode $1 $2 }

List :             { [] }
     | rule List   { $1:$2 }
     | token List  { $1:$2 }

{

runParser :: String -> Either String [Item]
runParser s = runP sadly ([MRoot], s)

happyError :: P a
happyError = do
    s <- getS
    throwError $ "Parse error. Rest of file: " ++ take 50 s

}
