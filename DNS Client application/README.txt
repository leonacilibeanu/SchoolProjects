Compilare:
make clean
make

Exemplu de rulare:
./my_dns_client google.com A


Informatii despre aplicatie

Aplicatia utilizeaza serverele dns in ordinea in care apar in dns_servers.conf
In cazul in care nu a primit nici un raspuns de la server timp de 2s, se utilizeaza serverul urmator din lista.
In cazul in care RCODE din headerul raspunsului este diferit de 0 (ceea ce inseamna ca serverul a transmis o eroare) se incearca urmatorul server din lista.
Daca nici unul dintre serverele din lista nu poate oferi informatii, atunci se afiseaza un mesaj de eroare.
Aplicatia trateaza urmatoarele erori:
- inexistenta fisierului dns_servers.conf
- imposibilitatea de a deschide pentru scriere fisierul "logfile"
- utilizarea numarului corect de argumente (2)
- aparitia unui tip necunoscut in cererea catre serverul DNS sau in raspuns
- neprimirea unui raspuns in maxim 2s
- primirea unui raspuns care contine un mesaj de eroare
