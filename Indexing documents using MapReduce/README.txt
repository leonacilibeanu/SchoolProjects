Tema 2
------


Utilizarea aplicatiei
---------------------

Aplicatia poate fi rulata astfel: java Main NT fisier_intrare fisier_iesire
Pentru ca aplicatia sa poata deschide documentele, in cazul in care in fisierul de intrare este specificat doar numele documentelor
(si nu calea completa), aplicatia trebuie rulata din acelasi director in care se afla documentele.

Functionarea aplicatiei
-----------------------

Aplicatia mentine un singur "pool" de thread-uri, care este reutilizat.
Clasa Master imparte problema principala in task-uri care sunt asignate worker-ilor prin intermediul unui ExecutorService.

Indexarea sectiunilor din documente este implementata in clasa DocumentParser, care este instantiata de catre workeri atunci cand au
de rulat un task de tipul MapTask.
Prima operatie de reducere centralizeaza in paralel statisticile pentru fiecare document, un task de tipul ReduceTask ocupandu-se de
centralizarea statisticilor corespunzatoare sectiunilor aferente unui anume document.
Cautarea (a doua operatie de reducere) se realizeaza in paralel in fiecare document, prin intermediul task-urilor de tipul SearchTask.

Pentru efectuarea cautarii am considerat relevanta unui document ca fiind suma frecventelor de aparitie 
in acel document ale cuvintelor cautate.
Cele mai relevante X rezultate ale cautarii sunt afisate in ordine alfabetica a numelor documentelor.
