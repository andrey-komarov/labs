{-# LANGUAGE TypeOperators #-}
module Utils where

import Datatypes

import Control.Arrow (second)

import Data.Char
import Data.List

import Data.Foldable (toList, Foldable)

import Data.Monoid

import Data.List

import qualified Data.Set as S
import Data.Set (Set)
import qualified Data.Map as M
import Data.Map (Map, (!))

type a ~> b = Map a b

eps = "EPS"
eof = "EOF"

isToken :: String -> Bool
isToken = isUpper . head

isRule :: String -> Bool
isRule = isLower . head

pp :: String -> String
pp s = "[" ++ s ++ "]"

ppx :: (Foldable f) => f String -> String
ppx s = unwords $ map pp $ toList s

dups :: (Foldable f, Ord a) => f a -> [a]
dups s = map head $ filter ((>=2) . length) $ group $ sort $ toList s

rule :: Rule -> [String]
rule (WithCode r _) = r
rule (WithoutCode r) = r

fixed :: (Eq a) => a -> (a -> a) -> a
fixed a f = let fa = f a in if fa == a then a else fixed fa f

epsProducing :: [Item] -> Set String
epsProducing items = fixed mempty step where
    step s = s <> S.fromList [from | RuleDecl from to <- items, 
               any (all (`S.member` s)) (map rule to)]

first :: [Item] -> (String ~> Set String)
first items = fixed initialM step where
    initial = [(t, [t]) | TokenDecl t _ _ <- items]
           ++ [(t, [eps]) | t <- toList (epsProducing items)]
           ++ [(from, [t]) | RuleDecl from to <- items,
                rr <- map rule to, not (null rr), let t = head rr, isToken t]
           ++ [(from, []) | RuleDecl from _ <- items]
    initialM = M.fromListWith (<>) $ map (second S.fromList) initial
    step m = M.unionWith (<>) m $ M.fromList $ [
         (from, m ! (head rr)) | RuleDecl from to <- items, rr <- map rule to, not (null rr)]
         ++ [(s1, m ! s2) | RuleDecl _ to <- items, rr <- map rule to, s1:s2:_ <- tails rr, 
             eps `S.member` (m ! s1)]

firstS :: (String ~> Set String) -> [String] -> Set String
firstS f xs = (S.delete eps $ mconcat [ f!last | 
                        last:rpref <- map reverse (inits xs), 
                        all (\s -> eps `S.member` (f!s)) rpref ]
              ) <> if all (\s -> eps `S.member` (f!s)) xs 
                    then S.singleton eps
                    else S.empty

follow :: [Item] -> String ~> Set String
follow items = fixed initial step where
    start = head [from | RuleDecl from _ <- items]
    e = epsProducing items
    f = first items
    epsSet = S.singleton eps
    initial = M.fromListWith (<>) $ [(start, S.singleton eof)] 
               ++ [(from, S.empty) | RuleDecl from _ <- items ]
               ++ [(tok, S.empty) | TokenDecl tok _ _ <- items ]
               ++ [(b, S.delete eps (firstS f rest)) | 
                   RuleDecl _ to <- items, rr <- map rule to, b:rest <- tails rr, isRule b]
    step :: (String ~> Set String) -> (String ~> Set String)
    step m = M.unionWith (<>) m $ M.fromListWith (<>) $ [(b, m!from) | 
            RuleDecl from to <- items, rr <- map rule to, 
            b:rest<-tails rr, eps `S.member` (firstS f rest)]
 
