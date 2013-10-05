Corectorul ortografic


Pentru rezolvarea acestei teme am implementat 6 functii si o clasa ajutatoare.

Clasa Candidate retine cuvantul/propozitia care se afla in curs de prelucrare, si informatii referitoare la proprietatile acesteia : frecventa si distanta minima de editare.
Clasa are:
	- constructor care primeste drept parametrii: frecventa, distanta minima de editare si structura
	-o functie show(), pe are am folosit-o in testarile intermediare


Initial, se citeste dictionarul de cuvinte, ale carui valori sunt pastrate intr-o structura de tip HashMap<String,Integer>. Astfel, pentru fiecare asociere cuvant-frecventa din dictionar, pastreaza o cheie, reprezentand cuvantul si o valoare echivalenta cu frecventa. 
Cuvantul ce urmeaza a fi corectat se citeste de la tastatura, se aplica asupra lui functia de eliminare a spatiilor, dupa care se trimite drept parametru functiei dynamicSearch.

Functia LevenshteinDistance, primeste drept parametrii 2 stringuri. Se va calcula distanta minima de editare pentru al doilea string fata de primul, folosind un algoritm de tip programare dinamica.
Returneaza o lista de tip Integer, cu valori ale distantelor minime de editare ale input si substringuri ale inputului.
Exemplu: Daca se calculeaza distanta minima de editare a structurii "soreserved" fata de de cuvantul "reserved", algoritmul va intoarce o lista cu distantele minime de editare a: "s", "so", "sor", "sore",...,"soreserved" fata de "reserved". In aceasta ordine.


Functia FindBest, creeaza initial o lista de candidati pentru substringuri ale input, avand minimum de proprietati: frecventa foarte mica, distanta foarte mare. Va returna aceasta lista de candidati, optimizata  prin apelul succesiv al functiei LevenshteinDistance pentru fiecare cuvant din dictionar. Valoarea de pe o anumita pozitie a listei va fi actualizata doar daca rezultatul intermediar obtinut trece de cel putin un filtru: distanta minima de editare, frecventa maxima, minim lexicografic.


Functia dynamicSearch, primeste drept parametru structura citita de la tastatura si incearca sa gaseasca o varianta mai buna a sa, conform dictionarului si a filtrelor impuse.
Creeaza o matrice de candidati superior triunghiulara, care este initializata cu valorile aferente apelului functiei FindBest pentru fiecare substring al input(extras de la i la n, unde i=0 to n-1). Matricea astfel initializata este supusa unui algoritm de tip programare dinamica, in care se realizeaza toate combinatiile posibile ale candidatilor din matrice pana cand se obtine o solutie optima. Se utilizeaza un candidat intermediar, combinatie a structurilor de pe pozitiile (i,k) si (k+1,j). Asemanator FindBest, daca acesta trece de unul din filtre: distanta minima, nr minim de cuvinte, frecventa maxima sau minim lexicografic, atunci elementul de pe pozitia (i,j) va fi actualizat.
La finalul acestui algoritm, se va obtine o matrice in care, in pozitia (1,n), se va afla chiar structura cautata. Se returneaza aceasta structura si se afiseaza.
