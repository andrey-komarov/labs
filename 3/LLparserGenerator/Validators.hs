module Validators where

import Datatypes
import Utils

import Control.Applicative
import Control.Monad.Writer
import Control.Monad.Error
import Control.Monad.State

import Data.List

import qualified Data.Set as S
import Data.Set (Set)
import qualified Data.Map as M
import Data.Map (Map)

checkAll :: [Item] -> Gen ()
checkAll r = do
    checkTokens r
    checkRules r

checkTokens :: [Item] -> Gen ()
checkTokens items = do
    tell [ "## Checking tokens..." ]
    let rhs = S.fromList [r | RuleDecl from rx <- items, rr <- map rule rx
            , r <- rr, isToken r]
    let lhs' = [t | TokenDecl t _ _ <- items]
    let lhs = S.fromList lhs'
    when (null lhs') $ do
        tell [ "* **Error**: no tokens? O'rly?" ]
        throwError "No tokens"
    unless (length lhs' == S.size lhs) $ do
        tell [ "* **Error**: duplicated declaration of token(s) "
             ++ ppx (dups lhs')]
        throwError "Duplicate tokens"
    when ("EOF" `S.member` lhs) $ do
        tell [ "* **Error**: <EOF> token is not permitted" ]
        throwError "<EOF> token is not permitted"
    when ("EPS" `S.member` lhs) $ do
        tell [ "* **Error**: <EPS> token is not permitted" ]
        throwError "<EPS> token is not permitted"
    tell [ "* Token declarations: " ++ ppx lhs
         , "* Tokens in use: " ++ ppx rhs
         , "* Tokens to skip: " ++ ppx [t | TokenDecl t _ False <- items]
         ]
    unless (S.null $ S.difference lhs rhs) $
        tell [ "* **Warning**: token(s) " ++ ppx (S.difference lhs rhs) ++ " are not used"]
    unless (rhs `S.isSubsetOf` lhs) $ do
        tell [ "* **Error**: no declaration for token(s) " ++ ppx (S.difference rhs lhs)]
        throwError "Not all tokens are declared"
    tell [""]

checkRules :: [Item] -> Gen ()
checkRules items = do
    tell [ "## Checking rules..." ]
    let rhs = S.fromList [r | RuleDecl name rx <- items, rr <- map rule rx
            , r <- rr, isRule r]
    let lhs' = [r | RuleDecl r _ <- items, isRule r]
    let lhs = S.fromList lhs'
    unless (length lhs' == S.size lhs) $ do
        tell [ "* **Error**: Duplicate declaration of rule(s) "
            ++ ppx (dups lhs') ]
        throwError "Duplicate rules"
    tell [ "* Rule declarations: " ++ ppx lhs
         , "* Rules in use: " ++ ppx rhs
         ]
    unless (S.null $ S.difference lhs rhs) $
        tell [ "* **Warning**: rule(s) " ++ ppx (S.difference lhs rhs) ++ " are not used"]
    unless (rhs `S.isSubsetOf` lhs) $ do
        tell [ "* **Error**: no declaration for token(s) " ++ ppx (S.difference rhs lhs)]
        throwError "Not all rules are declared"
    tell [""]

