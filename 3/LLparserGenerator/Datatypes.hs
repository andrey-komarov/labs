{-# LANGUAGE GeneralizedNewtypeDeriving #-}
module Datatypes where

import Lexer

import Control.Applicative
import Control.Monad.Writer
import Control.Monad.Error
import Control.Monad.State

import qualified Data.Set as S
import Data.Set (Set)
import qualified Data.Map as M
import Data.Map (Map)

data Item = TokenDecl String String Bool
          | RuleDecl String [Rule]
          | AttrDecl String String String
          | Com String
    deriving (Show)

data Rule = WithoutCode [String]
          | WithCode [String] String
    deriving (Show)


type GRule = (String, [[String]])

-- адок
newtype Gen a = Gen (ErrorT String (WriterT [String] (State (Map FilePath String))) a)
    deriving (Functor, Monad, MonadWriter [String], MonadError String,
                MonadState (Map FilePath String))

-- ещё адок
runGen :: Gen a -> (Either String a, [String], Map FilePath String)
runGen (Gen g) = let ((e, w), s) = (runState . runWriterT . runErrorT) g M.empty
                 in (e, w, s)


