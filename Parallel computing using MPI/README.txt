Tema 3


Compilarea pe FEP
------------------
Inainte de compilare trebuie rulata comanda "module load libraries/openmpi-1.6-gcc-4.4.6", altfel va fi utilizata implicit libraria MPICH in loc de OpenMPI.
Pentru a efectua compilarea pe FEP cu libraria OpenMPI se poate utiliza scriptul "build_on_fep.sh", care ruleaza automat aceasta comanda.

Rularea Programului
-------------------
Programul primeste ca parametri fisierul de intrare si fisierul de iesire.
Pentru a rula programul pe cluster, se utilizeaza un wrapper denumit "start.sh" care primeste ca parametri numarul de procese, fisierul de intrare si fisierul de iesire.
Un exemplu de rulare a programului cu acest wrapper se gaseste in fisierul "start_on_grid.sh".
