import Data.Char
import Data.List

--tipul de date folosit pentru afisarea rezultatul final

data Result = Match [Maybe String] | NoMatch
	deriving (Show)

--tipul de date folosit pentru stocarea informatiilor din sablon

data Regex = Elem Char | Group [[Regex]] | Dot | Repeat Regex | TakeInterval [Char] | DropInterval [Char]
	deriving (Show,Eq)

--functia "simpleParse" face o prima evaluare a sablonului, returnand o expresie de tip [Regex] in care trateaza toate
--cazurile, mai putin cazul special, Repeat, pentru care se va reevalua sablonul. Aceasta apeleaza pe rand "getGroup" sau
--"getInterval" in functie de aparitiile caracterelor "[" (semnaleaza prezenta unui interval), '(' (semnaleaza prezenta unui
--grup

simpleParse [] = []
simpleParse (x:xs)
	| (x == '[') && ((head xs) == '^') = (DropInterval (fst (getInterval (tail xs) "" 1))):(simpleParse (snd (getInterval (tail xs) "" 1)))    
	| (x == '[') && ((head xs) /= '^') = (TakeInterval (fst (getInterval xs "" 1))):(simpleParse (snd (getInterval xs "" 1)))
	| (x == '(') = (Group (map simpleParse (fst (getGroup xs [] [] 1)))):(simpleParse (snd (getGroup xs [] [] 1)))
	| (x == '.') = Dot:(simpleParse xs)
	| otherwise = (Elem x):(simpleParse xs)
	
	
-- (x:xs) = lista in curs de evaluare
-- grup = variabila temporara de stocare a unui grup la deschiderea unei paranteze
-- gather = variabila de stocare a unui grup de grupuri (lista de liste) gasit la parcurgerea listei primita ca parametru
-- nr de paranteze deschise (liste imbricate)

--functia "getGroup" returneaza o lista de liste de tip Regex, in functie de aparitiile caracterului '(' care influenteaza incrementarea lui n,
--si a caracterului ')', care influenteaza decrementarea lui n

getGroup xs grup gather 0 = ((reverse grup):gather,xs)
getGroup (x:xs) grup gather n
	| (x == '(') = (getGroup xs (x:grup) gather (n+1))
	| (x == ')') && (n>1) = (getGroup xs (x:grup) gather (n-1))
	| (x == ')') && (n==1) = (getGroup xs grup gather (n-1))
	| (x == '|') && (n==1) = (getGroup xs [] ((reverse grup):gather) n)
	| otherwise = getGroup xs (x:grup) gather n
	

--functia "generateInterval" genereaza o lista de tip Char in functie de capetele: first si last, primite drept parametru
--va genera o lista de elemente din interval, mai putin ultimul, care este adaugat in functia care o apeleaza: "getInterval"


generateInterval first last 
	| (first == last) = []
	| otherwise = ([first] ++ (generateInterval (chr ( (ord first) + 1)) last))

-- functia "getInterval"
-- (x:xs) = lista care contine intervalul
-- interval = variabila de stocare a intervalului
-- n = nr de paranteze deschise, in acest caz va fi intotdeauna egal cu 1 si se va decrementa la prima aparitie a caracterului  ']'
-- intoarce o variabila de tip pereche, prima reprezinta intervalul gasit si cea de-a doua, restul listei care urmeaza a fi evaluata

getInterval xs interval 0 = (interval, xs)	
getInterval (x:xs) interval n
	| ((head xs) == '-') = (getInterval (tail xs) (interval ++ (generateInterval x (head (tail xs)))) n)
	| ((head xs) == ']') =  (getInterval (tail xs) (interval ++ [x]) (n-1))
	| otherwise = (getInterval xs (interval ++ [x]) n)

-- functia "repeatParse" face o reevaluare a sablonului, pentru a stabili elementele care se repeta
-- in acest caz, se foloseste pattern matching, in functie de tipul elementelor din lista, deja evaluata 
-- in functia "simpleParse" 

repeatParse [] = []	
repeatParse (x:[]) = (x:[])	
repeatParse ((Group x):[]) = (Repeat (Group (map repeatParse x))):[]	
repeatParse ((Group x):y:xs)
	| (y == (Elem '*')) = (Repeat (Group (map repeatParse x))):(repeatParse xs)
	| otherwise = ((Group x):(repeatParse (y:xs)))
repeatParse (x:y:xs)
	| (y == (Elem '*')) = (Repeat x):(repeatParse xs)
	| otherwise = (x:(repeatParse (y:xs)))

-- functia "match" stabileste daca doua elemente, unul de tip Regex, si altul de tip Char, sunt compatibile
-- se foloseste pattern matching, in functie de tipul parametrului de tip Regex si constrangerile stabilite de acesta

match (Elem x) y = (x == y)

match Dot y = True

-- in cazul in care elementul din lista trimis drept parametru este de tip (TakeInterval s), se face o parcurgere a stringului,
-- daca se gaseste o variabila compatibila atunci se returneaza True, altfel False

-- pentru tipul (DropInterval s), se foloseste acelasi algoritm, mai putin valorile returnate, care se inverseaza

match (TakeInterval []) y = False
match (TakeInterval s) y
	|((head s) == y) = True
	|otherwise = (match (TakeInterval (tail s)) y)
	 
match (DropInterval []) y = True
match (DropInterval s) y
		|((head s) == y) = False
		|otherwise = (match (DropInterval (tail s)) y)

-- functia "matchGroup" verifica daca cel putin o lista din cadrul listei primita ca parametru este compatibila cu sirul
-- face append pentru lista din grup si restul listei, care urmeaza a fi comparata
-- reapeleaza "matchExpression" pentru a verifica structura nou obtinuta, daca aceasta reprezinta structura cautata
-- returneaza True, altfel continua cautarea in lista de liste a tipului Group		
matchGroup [] _  _= False
matchGroup (x:xs) r s
	| (matchExpression (x ++ r) s) = True
	| otherwise = (matchGroup xs r s)
	
-- functia "matchRepeat" primeste drept parametrii
-- x = expresia din cadrul (Repet x)
-- new_y = initial egala cu restul sablonului care urmeaza a fi evaluat, la care se adauga x pana cand, lista new_y va avea mult prea multe elemente,
-- 		   comparativ cu stringul s, pentru a fi luata in considerare
-- s = stringul a carui compatibilitate se verifica
-- n = lungimea lui s, care se decrementeaza pentru fiecare reapelare a functiei

matchRepeat x new_y s n
	| (n == 0) = False
	| (matchExpression new_y s) = True
	| otherwise = (matchRepeat x (x:new_y) s (n-1))
	
-- functia "matchExpression", primeste structura compilata, in urma apelului "simpleParse" si "repeatParse" si compara
-- fiecare element din xs cu elementul de pe aceeasi pozitie din ys, bineinteles in functie de tipul elementului x din
-- xs in curs de evaluare

matchExpression [] [] = True
matchExpression xs [] = False
matchExpression [] ys = False
matchExpression ((Group x):xs) y = (matchGroup x xs y)
matchExpression ((Repeat x):xs) y = (matchRepeat x xs y (length y))
matchExpression (x:xs) (y:ys) = ((match x y)&&(matchExpression xs ys))

-- functia "compileRegex" evalueaza daca cele doua stringuri primite de la tastatura sunt compatibile.
-- in caz afirmativ, returneaza Match [], altfel NoMatch

compileRegex struct input 
	| (matchExpression (repeatParse (simpleParse struct )) input) == True = Match []
	| otherwise = NoMatch