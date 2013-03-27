Segmentare de imagini

In rezolvarea temei am creat 4 clase:

	1. Clasa Graph, in care e definita structura grafului. Se retin informatii legate de: dimensiunea grafului (size); un HashMap pentru muchii(edges) a carui cheie este nodul sursa al muchiei si valoarea este o lista de tip Edge in care se retin nodul destinatie al muchiei si capacitatea; un vector de parinti de tip int (parents). In constructorul acestei clase, se fixeaza dimensiunea grafului si se initializeaza vectorul parents si lista edges.
	In clasa Graph exista 2 functii: addEdge care adauga muchii in edges in ambele sensuri pentru 2 noduri primite drept parametrii; show, functie de afisare a grafului.



	2.In clasa Graph este definita o subclasa, Edge, in care se retin informatii despre muchie (nod destinatie si capacitate muchie). Pentru clasa Edge este definit un constructor care seteaza valorea nodului si a capacitatii.



	3. Clasa Formule e formata din 4 functii (o functie pentru determinarea mediei valorilor, o functie pentru calculul deviatiei standard, o functie pentru calculul capacitatii muchiei sursa-nod, drena-nod si o functie pentru calculul capacitatii muchiei dintre doua noduri din graf);



	4.In clasa principala, Main sunt definite 5 functii + main:
		- functia createGraph returneaza graful corespunzator configuratiilor citite din fisiere. Se initializeaza graful prin apel de constructor. Se creaza o instanta a clasei Formule prin intermediul careia se calculeaza media valorilor, deviatia standard si in final se calculeaza capacitatile aferente fiecarei muchii, care apoi se adauga in graf prin apelul functiei addEdges din clasa Graph.
		- functia bfs primeste parametru graful in curs de prelucrare si returneaza o lista cu calea cea mai scurta intre sursa si drena. Se implementeaza algoritmul BFS pentru a gasi un drum intre sursa si destinatie. Se utilizeaza un vector de parinti pentru a verica daca un nod a fost sau nu adaugat in coada.
		- functia saturatePath primeste drept parametrii graful si calea de la sursa la drena, gasita prin apelul bfs. Cat timp mai exista un drum de la sursa la destinatie, se gaseste fluxul maxim pentru cale si se satureaza in ambele sensuri, graful creat fiind neorientat. Returneaza fluxul cu care s-a saturat drumul.
		- functia maxFlow primeste parametru un graf si returneaza fluxul maxim prin acest graf. Cat timp mai exista drumuri de crestere in graf (determinate cu functia bfs), acestea se satureaza (prin apelul functiei saturatePath) si se contorizeaza fluxul de saturare pentru fiecare astfel de drum.
		-in functia main, se citesc mai intai: date legate de valoarea pixelilor din "imagine.pgm" si se adauga in lista img; date referitoare la mastile de foreground si background din "mask_fg.pgm" si "mask_bg.pgm", stocate in listele maskF, respectiv maskB; valoarea parametrilor lambda si treshold din fisierul "parametri.txt".
Observatie: In citirea acestor parametri se considera ca fiind scrisi fiecare pe cate o linie.
Prin apelul functiei createGraph, se creeaza graful aferent datelor citite din fisiere. Apoi, se apeleaza functia maxFlow pentru a determina energia minima a configuratiei pe care o afisez in consola. In final, se apeleaza functia minCut pentru a obtine un vector de tip boolean in care, pentru pixelii considerati a fi in fundal, are valoarea true si pentru pixelii obiect are valoarea false. Pe baza acestui vector se realizeaza afisarea in fisierul "segment.pgm" a imaginii obtinute.
