\documentclass[11pt,a4paper,oneside]{article}

\usepackage[english,russian]{babel}
\usepackage[T2A]{fontenc}
\usepackage[utf8]{inputenc}
\usepackage{graphicx}
\usepackage{expdlist}
\usepackage{mfpic}
\usepackage{comment}
\usepackage{listings}
\usepackage{epigraph}
\usepackage{url}
\usepackage{array}
%include polycode.fmt
%options ghci
%format Var v = v
%format App x y = x "\times" y
%format `App` = " \cdot "
%format Lam x = "\lambda" x "\to"
%format if' = if
%format then' = then
%format else' = else
%format $ = " "

\long\def\ignore#1{}

\begin{document}

\ignore{
\begin{code}
import Prelude hiding (id, succ, pred, and, or, catch, head, tail)
import qualified Prelude as P
\end{code}
}

\ignore{
\begin{code}
import Control.Exception
import Control.Monad.State
import qualified Data.Map as M
import Data.Maybe (fromMaybe)

type Variable = String
data Term = Var Variable | Lam Variable Term | App Term Term deriving (Ord)

instance Show Term where
    show (Var x) = x
    show (Lam x e) = "\\" ++ x ++ "." ++ show e
    show (App e1@(Lam _ _) e2) = "(" ++ show e1 ++ ") (" ++ show e2 ++ ")"
    show (App e1 (Var v)) = show e1 ++ " " ++ v
    show (App e1 e2) = show e1 ++ " (" ++ show e2 ++ ")"

free (Var v)    = [ v ]
free (Lam v t)  = filter (/= v) . free $ t
free (App t t') = (free t) ++ (free t')

subst :: Term -> Variable -> Term -> Term
subst t@(Var v)   var what = if v == var then what else t
subst t@(Lam v b) var what = if v == var then t else Lam v (subst b var what)
subst (App t t')  var what = App (subst t var what) (subst t' var what)

newname :: [Variable] -> Variable -> Variable 
newname fv = P.head . filter (not . flip elem fv) . iterate ('_':)

deny:: [String] -> Term -> Term
deny d (Var v) = Var v -- Var $ newname d v
deny d (Lam v t) = Lam v' $ subst t v (Var v') where v' = newname (d ++ free t) v
deny d (App t1 t2) = App (deny d t1) (deny d t2)

unifyBound :: Term -> Term 
unifyBound e = fst $ runState (unifyBound' e) (M.empty, free e)

unifyBound' :: Term -> State (M.Map Variable Variable, [Variable]) Term
unifyBound' (Var v) = do
    (m, used) <- get
    return $ Var $ fromMaybe v (M.lookup v m)
unifyBound' e@(App t1 t2) = do
    t1' <- unifyBound' t1
    t2' <- unifyBound' t2
    return $ App t1' t2'
unifyBound' (Lam x t) = do
    (m, used) <- get
    let new = fst $ runState unused used
    put (M.insert x new m, new:used)
    e <- unifyBound' t
    (_, used') <- get
    put (m, used')
    return $ Lam new e

eq :: Term -> Term -> Bool
eq (Var v) (Var v') = v == v'
eq (Lam v1 e1) (Lam v2 e2) = v1 == v2 && e1 `eq` e2
eq (App e1 e2) (App e1' e2') = e1 `eq` e1' && e2 `eq` e2'
eq _ _ = False

instance Eq Term where
    e1 == e2 = free e1 == free e2 && newE1 `eq` newE2 where
        newE1 = unifyBound e1
        newE2 = unifyBound e2
        fv = free e1

unused :: State [Variable] Variable
unused = do
    used <- get
    let new = P.head $ filter (not . (`elem` used)) $ map show [1..]
    put $ new:used
    return new 

no :: Integer -> Term -> Term

no n t = no' n ({- unifyBound -} t)

no' 0 t = error $ "Too long sequence at [" ++ show t ++ "]"
no' n t 
    | reduced = no (n - 1) rest
    | otherwise = t
    where (reduced, rest) = noOne t

noOne :: Term -> (Bool, Term)
noOne (App (Lam v t1) t2) = (True, subst (deny (free t2) t1) v t2)
noOne t@(Var v) = (False, t)
noOne (Lam v t) = (reduced, Lam v rest) 
    where (reduced, rest) = noOne t 
noOne t@(App t1 t2) = if reducedLeft 
                        then (True, App t1' t2) 
                        else if reducedRight
                            then (True, App t1 t2')
                            else (False, t)
    where
        (reducedLeft, t1') = noOne t1
        (reducedRight, t2') = noOne t2
        fv = free t2

sa n t = sa' n $ {- unifyBound -} t

sa' 0 t = error $ "Too long sequence at [" ++ show t ++ "]"
sa' n t 
    | reduced = sa (n - 1) rest
    | otherwise = t
    where (reduced, rest) = saOne t 

saOne :: Term -> (Bool, Term)
saOne t@(Var v) = (False, t)
saOne (Lam v t) = (reduced, Lam v rest)
    where (reduced, rest) = saOne t
saOne t@(App t1 t2) = if reducedRight
        then (True, App t1 t2')
        else case t1 of 
            (Lam v t1'') -> (True, subst (deny (free t2) t1'') v t2) where
                (_, t1''') = saOne t1'' 
            _ -> (reducedLeft, App t1' t2)
    where 
        (reducedRight, t2') = saOne t2
        (reducedLeft, t1') = saOne t1

normIO :: Int -> Term -> IO Term
normIO step t = do
    let (reduced, rest) = noOne t
    print $ "================" ++ " step " ++ show step ++ " ======== "
    if reduced
        then normIO (step + 1) rest
        else do 
            print rest
            return rest

norm :: Term -> Term
norm = no 1000

toInt :: Term -> Int
toInt (Var v) = 0
toInt (Lam v x) = toInt x
toInt (App v1 v2) = (toInt v1) + (toInt v2) + 1

\end{code}
}


\section{Лямбда-исчисление}

\emph{Лямбда-исчисление}~--- формальная система, придуманная в 1930-х годах 
Алонзо Чёрчем. Лямбда-функция является, по сути, анонимной функцией.
Эта концепция показала себя удобной и сейчас активно используется во многих
языках программирования.

Более формально, \emph{лямбда-функцию} (или, \emph{лямбда-терм}) можно задать 
следующей грамматикой:

$$
\begin{array}{r c l}
\langle Term \rangle & ::= & \langle Variable \rangle \\
                       & || & \langle Term \rangle \langle Term \rangle \\
                       & || & \lambda \langle Variable \rangle \to \langle Term \rangle\\
                       & || & ( \langle Term \rangle )\\
\langle Variable \rangle & ::= & \langle Char \rangle *\\
\end{array}
$$

В первом случае функция является просто переменной. 
Во втором происходит \emph{аппликация} (\emph{применение}) одной функции к другой.
Это аналогично вычислению функции-левого операнда на аргументе-правом операнде.
В третьем~--- \emph{абстракция} по переменной. В данном случае происходит 
создание функции одного аргумента с заданными именем аргумента и телом функции.

Рассмотрим, например, функцию |id = \x -> x|. Эта функция принимает аргумент и 
возвращает его неизменённым. Например, 
|id 2 == | \eval{id 2}. Аналогично, |id y == y|. 

Ещё один пример функции: |sum = \x -> \y -> x + y|. Эта функция двух аргументов,
которая возвращает их сумму. Правда, здесь мы немного вышли за написанную выше грамматику.
Ну да ладно. |sum 2 3 == 5|

\subsection{Приоритет операций}
\begin{itemize}
\item Применение левоассоциативно: |x y z w == ((x y) z) w|
\item Аппликация забирает себе всё, до чего дотянется:\\ 
    |\x -> \y -> \z -> z y x == \x -> (\y -> (\z -> ((z y) x)))|
\item Скобки играют привычную роль группировки действий
\end{itemize}

\subsection{Свободные и связанные переменные}
\emph{Связанными} переменными называются все переменные, по которым выше в 
дереве разбора были абстракции. Все остальные переменные называются свободными.

Например, в |\x -> \y -> x|, |x| связана, а |y|~--- свободна. А в |\y -> x (\x -> x)|
в своём первом вхождении переменная |x| свободна, а во втором~--- связана.

Рассмотрим функции |\y -> y| и |\x -> y|. В первой из них при взгляде на |y|
понятно, что она имеет отношение к переменной, по которой производилась 
абстракция. Если по одной и той же
переменной абстракция производилась более одного раза, то переменная связана
с самым поздним (самым нижним в дереве разбора) абстрагированием. Например, в
|\x -> \x -> \y -> \x -> x|, переменная |x| связана с самой правой абстракцией 
по |x|.

\subsection{$\alpha$-конверсия}

Рассмотрим функции |(\x -> x) z| и |(\y -> y) z|. Интуитивно понятно, что они 
являются одинаковыми. 

\emph{$\alpha$-конверсия}~--- переименование связанной переменной. Выражение
|\x -> f| можно заменить на |\y -> f[x := y]|, если |y| не входит свободно в |f|,
где $f[x:=y]$ означает замену всех свободных вхождений $x$ в $f$ на $y$.

Функции, получающиеся одна из другой с помощью $\alpha$-конверсий, называются 
\emph{$\alpha$-эквивалентными} и обозначаются $f \equiv_\alpha g$.

Функции |\x -> \y -> x y z| и |\a -> \x -> a x z| являются $\alpha$-эквивалентными,
а |\x -> \y -> y z| и |\y -> \x -> y z|~--- нет.

\subsection{$\beta$-редукция}
$\beta$-редукция олицетворяет идею счёта значения функции. Выражение вида 
|(\x -> f) y| можно заменить на $f[x := y]$, где $f[x:=y]$, как и ранее, означает
замену всех свободных вхождений $x$ в $f$ на $y$.

Через $f \to_\beta g$ обозначают сведение $f$ к $g$ с помощью одной $\beta$-редукции.
А через $f \to_\beta^* g$~--- за ноль или более.

\subsection{$\eta$-редукция}
Рассмотрим выражение вида |\x -> f x|. Если подставить в эту функцию значение 
$y$, то получим: $(\lambda x \to f x) y \to_\beta f y$. Но если просто подставить
$y$ в $f$, то получится то же самое. 

$\eta$-редукция~--- преобразование |\x -> f x| в $f$.

\section{Нотация Де Брюина}
Существует также альтернативное эквивалентное определение $\lambda$-исчисления.
В оригинальном определении для обозначения переменных использовались имена,
и была проблема с тем, что не были запрещены одинаковые имена в разных
абстракциях. 

От этой проблемы можно избавиться следующим образом. Вместо имени переменной 
будет храниться натуральное число~--- количество абстракций в дереве разбора,
на которое нужно подняться, чтобы найти ту лямбду, с которой данная переменная 
связана. В данной нотации получаются несколько более простые определения 
свободных переменных и $\beta$-редукции. 

Переменная называется свободной, если ей соответствует число, которое больше
количества абстракций на пути до неё в дереве разбора.

При $\beta$-редукции же нужно будет ко всем свободным переменным заменяющего 
дерева при каждой замене прибавить число, равное разницы уровней раньше и сейчас.
Это будет соответствовать тому, что эта переменная продолжит <<держаться>> за
ту же лямбду, что и раньше.

\section{Нумералы Чёрча}

\subsection{Определение}
Введём на основе лямбда-исчисления аналог натуральных чисел, основанный на идее, 
что натуральное число~--- это или ноль, или увеличенное на единицу натуральное 
число.

\begin{code}
zero' = Lam "s" $ Lam "z" $ Var "z"
one' = Lam "s" $ Lam "z" $ Var "s" `App` Var "z"
two' = Lam "s" $ Lam "z" $ Var "s" `App` (Var "s" `App` Var "z")
three' = Lam "s" $ Lam "z" $ Var "s" `App` (Var "s" `App` (Var "s" `App` Var "z"))
\end{code}

Каждое число будет функцией двух аргументов: какой-то функции и начального значения.
Число $n$ будет $n$ раз применять функцию к начальному значению и возвращать 
результат. Если такому <<числу>> дать на вход функцию $(+1)$ и $0$ в качестве 
начального значения, то на выходе как раз будет ожидаемое от функции число:
|three (+1) 0 == | \eval{three (+1) 0}.

\subsection{+1}
Функция, прибавляющая 1 к числу, должна принимать первым аргументом число.
Но число~--- функция двух аргументов. Значит, эта функция должна принимать три
аргумента: <<число>> $n$, которое хочется увеличить, функция, которую надо будет
$n+1$ раз применить, и начальное значение.

\begin{code}
succ' = Lam "n" $ Lam "s" $ Lam "z" $ Var "s" `App` (Var "n" `App` Var "s" `App` Var "z")
\end{code}

Здесь $n s z$~--- $n$ раз применённая к $z$ функция $s$. Но нужно применить $n+1$ 
раз. Отсюда $s (n s z)$.

\subsection{Сложение}
Сложение двух чисел похоже на прибавление единицы. Но только надо прибавить не
единицу, а второе число.

\begin{code}
plus' = Lam "n" $ Lam "m" $ Lam "s" $ Lam "z" $ Var "n" `App` Var "s" `App` (Var "m" `App` Var "s" `App` Var "z")
\end{code}

 <<$n$ раз применить $s$ к применённому $m$ раз $s$ к $z$>>

|(plus three three) (+1) 0 == | \eval{(plus three three) (+1) 0}

\subsection{Умножение}
Умножение похоже на сложение, но прибавлять надо не единицу, а второе число.
Или, в терминах нумералов Чёрча, в качестве применяемой несколько раз
функции должна быть не $s$, а функция, применяющая $n$ раз $s$.

\begin{code}
mult' = Lam "n" $ Lam "m" $ Lam "s" $ Var "n" `App` (Var "m" `App` Var "s")
\end{code}

Здесь |m s|~--- функция, которая $m$ раз применит $s$ к тому, что дадут ей на 
вход. С помощью $\eta$-редукции можно немного сократить эту формулу

\begin{spec}
mult = \n -> \m -> \s -> n (m s)
\end{spec}

|(mult three three) (+1) 0 == | \eval{(mult three three) (+1) 0}

\subsection{Возведение в степень}
It's a kind of magic

\begin{code}
power' = Lam "n" $ Lam "m" $ Var "m" `App` Var "n"
\end{code}

|(power three (succ three)) (+1) 0 == | \eval{(power three (succ three)) (+1) 0}

\subsection{Логические значения}
\begin{code}
true' = Lam "a" $ Lam "b" $ Var "a"
false' = Lam "a" $ Lam "b" $ Var "b"
\end{code}

Функции двух аргументов, возвращающие первый и второй, соответственное, аргументы.
Забавный факт: $false \equiv_\alpha zero$. Эти функции сделаны такими для того, 
чтобы красиво написать функцию $if$:

\begin{code}
if'' = Lam "p" $ Lam "t" $ Lam "e" $ Var "p" `App` Var "t" `App` Var "e"
\end{code}

Если ей в качестве первого аргумента дадут $true$, то вернётся $t$, иначе~--- $e$.

Стандартные функции булевой логики:

\begin{code}
and' = Lam "n" $ Lam "m" $ if'' `App` Var "n" `App` Var "m" `App` false'
or' = Lam "n" $ Lam "m" $ if'' `App` Var "n" `App` true' `App` Var "m"
not' = Lam "b" $ if'' `App` Var "b" `App` false' `App` true'
\end{code}

Ещё одной важной функцией является функция проверки, является ли число нулём:

\begin{code}
isZero' = Lam "n" $ Var "n" `App` (Lam "c" false') `App` true'
\end{code}

Функция выглядит несколько странно. |\c -> false|~--- функция, которая независимо
от того, что ей дали на вход, возвращает $false$. Тогда, если в качестве $n$
будет дан ноль, то функция, по определению нуля, не выполнится ни разу, и будет
возвращено значение по умолчанию $true$. Иначе же функция будет запущено, и 
вернётся $false$.

\subsection{Пара}

\begin{code}
pair' = Lam "a" $ Lam "b" $ Lam "t" $ Var "t" `App` Var "a" `App` Var "b"
fst'' = Lam "p" $ Var "p" `App` true'
snd'' = Lam "p" $ Var "p" `App` false'
\end{code}

Функция $pair$ принимает два значения и запаковывает их в пару так, чтобы к
ним можно было обращаться по $fst$ и $snd$. В $fst$ и $snd$ вместо $t$ в $pair$
будет подставлено $true$ или $false$, возвращающие, соответственно, первый и
второй аргументы, то есть $a$ или $b$, соответственно.

\subsection{Вычитание}
В отличие от всех предыдущих функций, вычитание для натуральных чисел определено
только в случае, если уменьшаемое больше вычитаемого. Положим в противном случае
результат равным нулю. Пусть уже есть функция, которая вычитает из числа единицу.
Тогда на её основе легко сделать, собственно, вычитание.

\begin{code}
minus' = Lam "n" $ Lam "m" $ Var "m" `App` pred' `App` Var "n"
\end{code}

 <<$m$ раз вычесть единицу из $n$>>.

Осталось, собственно, функция для вычитания единицы. Однако, это не так просто,
как может показаться на первый взгляд. Проблема в том, что, имея функцию, которую
нужно применить для того, чтобы продвинуться вперёд, продвинуться назад будет
проблематично. Если попробовать воспользоваться идеей о том, чтобы, начав от 
нуля, идти вперёд, и пройти на один шаг меньше, то будет не очень понятно, как
же остановиться ровно за один шаг до конца. Для реализации вычитания единицы 
сделаем следующее. $n$ раз выполним следующее: имея пару $(n-1, n-2)$ построим пару
$(n, n-1)$. Тогда после $n$ шагов во втором элементе пары будет записано число
$n-1$, которое и хочется получить.
% --pred = \n -> \s -> \z -> n (\g -> \h -> h (g s)) (\u -> z) (\u->u)
% --pred = \n -> \s -> \z -> snd (n (\p -> (pair (s (fst p)) (fst p))) (pair z z))
% --pred = \n -> \s -> \z -> fst' (n (\p -> pair (s (fst p))(fst p)) (pair z z))

\begin{code}
pred' = Lam "n" $ 
        Lam "s" $ 
        Lam "z" $ 
            snd'' `App` (Var "n" 
                            `App` ( 
                                     Lam "p" $ pair' 
                                        `App` (Var "s" `App` (fst'' `App` Var "p"))
                                        `App` (fst'' `App` Var "p")
                                  )
                            `App` ( pair' `App` Var "z" `App` Var "z" )
                        )
\end{code}

Если вы ничего не поняли, не огорчайтесь. Вычитание придумал Клини, когда 
ему вырывали зуб мудрости. А сейчас наркоз уже не тот!

\subsection{Сравнение}
Так как вычитание определено таким способом, чтобы для случая, в котором
уменьшаемое больше, чем вычитаемое, возвращать ноль, можно определить
сравнение на больше-меньше через него. Равными же числа $a$ и $b$ считаются, 
если $a - b = 0 \wedge b - a = 0$.

\begin{code}
le' = Lam "n" $ Lam "m" $ isZero' `App` (minus' `App` Var "n" `App` Var "m")
less' = Lam "n" $ Lam "m" $ le' `App` Var "n" `App` (pred' `App` Var "m")
eq' = Lam "n" $ Lam "m" $ and' 
            `App` (isZero' `App` (minus' `App` Var "n" `App` Var "m"))
            `App` (isZero' `App` (minus' `App` Var "m" `App` Var "n"))
\end{code}

\subsection{Комбинатор неподвижной точки}
Попробуем выразить в лямбда-исчислении какую-нибудь рекурсивную функцию.
Напрмер, факториал.

\begin{spec}
fact = \x -> if (isZero x) one (fact (pred x))
\end{spec}

Мы столкнулись с проблемой. В определении функции $fact$ используется функция 
$fact$. При формальной замене, получим бесконечную функцию. Можно попытаться решить 
эту проблему следующим образом

\begin{spec}
fact = (\f -> \x -> if (isZero x) one (f (pred x))) fact
\end{spec}

\emph{Неподвижной точкой} лямбда-функции $f$ назовём такую функцию $x$, что
$f x \to_\beta x$. Лямбда исчисление обладаем замечательным свойством: у каждой
функции есть неподвижная точка!

Рассмотрим такую функцию. 

\begin{code}
fix' = Lam "f" $   (Lam "x" $ Var "f" `App` (Var "x" `App` Var "x"))
            `App`  (Lam "x" $ Var "f" `App` (Var "x" `App` Var "x"))
\end{code}

Заметим, что $fix' \to_\beta \lambda f \to f ((\lambda x \to f (x x)) (\lambda x \to f (x x)))$.
Или, что то же самое, 
$\lambda f \to (\lambda x \to f (x x)) (\lambda x \to f (x x)) \to_\beta
 \lambda f \to f ((\lambda x \to f (x x)) (\lambda x \to f (x x)))$

Рассмотрим функцию

\begin{code}
fact'' = Lam "f" $ Lam "x" $ if'' `App` (isZero' `App` Var "x") 
                                  `App` one'
                                  `App` (mult' `App` Var "x" `App` (Var "f" `App`
                                            (pred' `App` Var "x")))
\end{code}

Как было показано выше, $fix f \to_\beta f (fix f)$, то есть, 
$fix fact'' \to_\beta fact'$, где $fact'$~--- искомая функция, считающая факториал.

\begin{code}
fact' = fix' `App` fact''
\end{code}

Это даст функцию, которая посчитает факториал числа. Но делать она это будет 
мееедленно-меееедленно. Для того, чтобы посчитать $5!$ потребовалось сделать
66066 $\beta$-редукций.

Тут правда ничего не понятно? :'(

\subsection{Деление}
Воспользовавшись идеей о том, что можно делать рекурсивные функции, сделаем 
функцию, которая будет искать частное двух чисел.

\begin{code}
div'' = Lam "div" $ Lam "n" $ Lam "m" $ if'' `App` (less' `App` Var "n" `App` Var "m")
         `App` zero'
         `App` (succ' `App` (Var "div" `App` (minus' `App` Var "n" `App` Var "m") `App` Var "m"))
div' = fix' `App` div''
\end{code}

И остатка от деления

\begin{code}
mod'' = Lam "mod" $ Lam "n" $ Lam "m" $ if'' `App` (less' `App` Var "n" `App` Var "m")
         `App` Var "n"
         `App` (Var "mod" `App` (minus' `App` Var "n" `App` Var "m") `App` Var "m")
mod' = fix' `App` mod'';
\end{code}

\subsection{Проверка на простоту}

$isPrimeHelp$~--- принимает число, которое требуется проверить на простоту и 
то, на что его надо попытаться поделить, перебирая это от 2 до $p-1$. Если на 
что-нибудь разделилось, то число~--- составное, иначе~--- простое.

\begin{code}
isPrimeHelp' = Lam "f" $ Lam "p" $ Lam "i" $ if'' `App` (le' `App` Var "p" `App` Var "i")
                `App` true'
                `App` ( if'' `App` (isZero' `App` (mod' `App` Var "p" `App` Var "i"))
                         `App` false'
                         `App` (Var "f" `App` Var "p" `App` (succ' `App` Var "i"))
                )
isPrimeHelp = fix' `App` isPrimeHelp'
isPrime = Lam "p" $ isPrimeHelp `App` Var "p" `App` two'
\end{code}

Следующее простое число. $nextPrime'$~--- следующее, больше либо равное заданного,
$nextPrime$~--- следующее, большее заданного.

\begin{code}
nextPrime'' = Lam "f" $ Lam "p" $ if'' `App` (isPrime `App` Var "p")
                                   `App` Var "p"
                                   `App` (Var "f" `App` (succ' `App` Var "p")) 
nextPrime' = fix' `App` nextPrime''
nextPrime = Lam "p" $ nextPrime' `App` (succ' `App` Var "p")
\end{code}

$ithPrimeStep$ пропрыгает $i$ простых чисел вперёд. $ithPrime$ принимает число
$i$ и пропрыгивает столько простых чисел вперёд, начиная с двойки.

\begin{code}
ithPrimeStep' = Lam "f" $ Lam "p" $ Lam "i" $ if'' `App` (isZero' `App` Var "i")
                `App` Var "p"
                `App` (Var "f" `App` (nextPrime `App` Var "p") `App` (pred' `App` Var "i"))
ithPrimeStep = fix' `App` ithPrimeStep'
ithPrime = Lam "i" $ ithPrimeStep `App` two' `App` Var "i"
\end{code}

...и всего через 314007 $\beta$-редукций вы узнаете, что третье простое число~---
семь!

\subsection{Списки}

Для работы со списками чисел нам понадобятся следующие функции:
\begin{itemize}
    \item $empty$~--- возвращает пустой список
    \item $cons$~--- принимает первый элемент и оставшийся список, склеивает их
    \item $head$~--- вернуть голову списка
    \item $tail$~--- вернуть хвост списка
\end{itemize}

Список будем хранить в следующем виде: $\langle len, p_1^{a_1}p_2^{a_2}\ldots p_{len}^{a_{len}} \rangle$. При этом, голова списка будет храниться как показатель степени при $p_{len}$.

\begin{code}
empty = pair' `App` zero' `App` one'
cons = Lam "h" $ Lam "t" $ pair' `App` (succ' `App` (fst'' `App` Var "t"))
        `App` (mult' `App` (snd'' `App` Var "t") `App` (power' 
                `App` (ithPrime `App` (fst'' `App` Var "t"))
                `App` Var "h"
        ))
head = Lam "list" $ getExponent `App` (snd'' `App` Var "list")
                                `App` (ithPrime `App` (pred' `App` (fst'' `App` Var "list")))

tail = Lam "list" $ pair' `App` (pred' `App` (fst'' `App` Var "list"))
                          `App` (eliminateMultiplier `App` (snd'' `App` Var "list")
                                  `App` (ithPrime `App` (pred' `App` (fst'' `App` Var "list" )))
                                )

eliminateMultiplier' = Lam "f" $ Lam "n" $ Lam "m" $ if'' `App` (isZero' `App` (mod' `App` Var "n" `App` Var "m"))
                        `App` (Var "f" `App` (div' `App` Var "n" `App` Var "m") `App` Var "m")
                        `App` Var "n"
eliminateMultiplier = fix' `App` eliminateMultiplier'

getExponent' = Lam "f" $ Lam "n" $ Lam "m" $ if'' `App` (isZero' `App` (mod' `App` Var "n" `App` Var "m"))
                `App` (succ' `App`(Var "f" `App` (div' `App` Var "n" `App` Var "m") `App` Var "m"))
                `App` zero'
getExponent = fix' `App` getExponent'
\end{code}

На основе этого всего уже можно реализовать эмулятор машины тьюринга:
с помощью пар, списков чисел можно хранить состояния. С помощью рекурсии можно
обрабатывать переходы. Входная строка будет даваться, например, закодированной
аналогично списку: пара из длины и числа, характеризующего список степенями 
простых. Я бы продолжил это писать, но уже на операции $head [1, 2]$ я не 
дождался окончания выполнения. Скорость лямбда-исчисления как вычислителя
печальна.  

\ignore{
\begin{code}

four' = norm $ succ' `App` three'
five' = norm $ succ' `App` four'

list2 = norm $ cons `App` one' `App` empty
list32 = cons `App` zero' `App` list2

normFiveFact = normIO 0 $ fact' `App` five'

-- fiftysix = mult twentyeight two
-- fiftyfive = pred fiftysix
-- six = pred seven

--main = do print $ fiftysix (+1) 0
main = do
 --   f <- normIO 0 $ ithPrime `App` three'
 --   f <- normIO 0 $ getExponent `App` (norm $ plus' `App` four' `App` four') `App` two'
    f <- normIO 0 $ head `App` (tail `App` list32)
    print $ toInt f
\end{code}
}

\end{document}
