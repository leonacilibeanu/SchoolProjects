Compilarea pe FEP
------------------
Inainte de compilare trebuie rulata comanda "module load libraries/openmpi-1.6-gcc-4.4.6", altfel va fi utilizata implicit libraria MPICH in loc de OpenMPI.
Pentru a efectua compilarea pe FEP cu libraria OpenMPI se poate utiliza scriptul "build_on_fep.sh", care ruleaza automat aceasta comanda.


Rularea Programului
----------------------
Programul primeste ca argumente fisierul de topologie si fisierul cu mesaje (in aceasta ordine).
Pentru a rula programul pe cluster, se utilizeaza un wrapper denumit "start.sh" care primeste ca parametri numarul de procese, fisierul de intrare si fisierul de iesire.
Un exemplu de rulare a programului cu acest wrapper se gaseste in fisierul "start_on_grid.sh".

Afisarea Datelor
----------------
Dupa determinarea topologiei este afisata matricea de adiacenta.
Tabelele de rutare ale fiecarui nod sunt afisate ca vectori (valoarea de pe pozitia i din vector reprezinta next-hop pentru pachetele care au ca destinatie nodul i).
Tabelele de routare sunt calculate pe baza matricei de adiacenta cu algoritmul BFS (breadth-first search) adaptat pentru grafuri.

Mesajele catre ajung la noduri sunt afisate cu un prefix: "[REC]" daca mesajul a ajuns la destinatie, sau "[FWD]" daca acest nod ruteaza mesajul respectiv.
Functionalitatea de broadcast este implementata prin trimiterea mesajului catre toti vecinii directi (ca in majoritatea retelelor clasice, mesajele de broadcast nu sunt propagate prin routere).

Dupa terminarea etapei de votare fiecare proces afiseaza rezultatul votului sub forma a 2 numere: liderul principal si liderul adjunct.

Datele sunt redirectionate catre stderror pentru a beneficia de line-buffering, care este implementat pe majoritatea platformelor pentru acest stream.