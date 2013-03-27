Tema3:

Tema e alcatuita din 10 clase si 2 interfete. Clasa principala a arborelui de parsare este Node
care reprezinta un schelet pentru constructia operatiilor si a operanzilor. Aceasta reuneste
campurile de retinere a liniei,coloanei si a listei de descendenti care nu va fi utilizata pentru
tipuri de date, Campul tip va fi utilizat doar pentru semanticVisitor pentru operatori pentru decizia
asupra tipului de date aflate in subordinea acestuia(-2 pentru conflict, -1 nedefinit, 0 boolean, 1 pentru
intreg) si metode specifice precum afisarea continutului unui nod care e facuta in functie de tipul
nodului, si o functie recursiva de afisare arbore ce utilizeaza metoda anterior precizata pentru
a-l afisa indentat.Clasele derivate din Node pe langa constructori specifici, mai contin campuri
specifice care sunt incapsulate folosind metode getter si suprascrieri a metodei accept. Clasa Main
contine metodele de constructie arbore, precum construiesteArbore si adaugaOperator. Metoda ConstruiesteArbore
e o implementare a algoritmului referit in enuntul temei, citind linie cu linie, spargand stringurile in
arrayuri, parsand informatia in obiecte de tip de baza Node, care sunt adaugate intr-o stiva de operatori
sau de operanzi, respectand regula din algoritm cu privirea la adaugarea unei adunari dupa o inmultire,
trebuind mai intai efectuate operatiile de inmultire anterioare pentru respectarea ordinii operatiilor.
Dupa parsarea unei intregi linii se scot operanzii din stiva cu cate un operator si re formeaza un subarbore
care se adauga in stiva de Operanzi, pas care se repeta si astfel se ajunge la final la un subarbore a 
liniei de cod care se adauga la radacina generica. Clasa SemanticVisitor contine pe langa constructori
si campurile specifice, implementarile metodelor din interfata visitor. Pentru radacina generica, 
se ia subarbore cu subarbore(deci linie cu linie), si se verifica corectitudinea fiecare in parte, prin
apel de acceptare vizitator, care umple lista definita cu operanzi si operatori, lista utilizata pentru
a verifica corectitudinea semantica. Clasa ResultVisitor functioneaza asemanator cu SemnaticVisitor,
doar ca Mapul este utilizat pentru valorile finale si nu pentru tipuri, iar lista pentru efectuarea
operatiilor si nu verificarea corectitudinii acestora.
