Tema 1 - Fotbal Static

	Clasa Ball descrie mingea, cu atributele ei si 2 functii ajutatoare. Aceasta contine: obiectul minge; obiectul ultimului jucator cu care s-a intersectat, penultima translatie, folosita pentru ciocnirile cu manta si pastrarea vitezei constante a mingei; variabila foundPlayer care verifica daca mingea este sau nu in contact cu un jucator; variabila darkenedContact asigura o singura modificare a culorii pentru fiecare stare a jucatorului: in cazul contactului cu mingea si a pasarii acesteia; o functie care seteaza campul contact si o functie care seteaza l1 si l2, penultima tranlatie pe x, respectiv y.

	Pentru initializarea terenului am folosit functia init(), unde am construit obiectele pe care ulterior le-am adaugat la sistemul de coordonate.

	In functia onIdle verific starea jocul pentru fiecare frame. Se trateaza mai intai exceptiile:
		- daca mingea a intrat in contact cu un jucator, se schimba culoarea jucatorului, la fel si daca mingea pleaca de la un jucator culoarea acestuia va reveni la culoarea initiala. Se folosesc functiile addShade si removeShade pentru aceste schimbari; 
		- daca au fost inscrise 3 goluri se inchide culoarea ecranului prin modificarea culorii fiecarui obiect;
		- daca una din echipe a inscris 3 goluri pe frame-ul anterior, se pastreaza culoarea schimbata a ecranului timp de 3 secunde, dupa care acesta revine la culorile initiale si se apeleaza functia reset(). Functia reset(): reinitializeaza scorul ambelor echipe la 0, inlocuieste in tabela de scor obiectele existente cu cele echivalente cu 0, seteaza pozitia initiala de selectie, muta jucatorii in pozitia initiala de plecare,  alege random un portar care se va afla in posesia mingei la inceperea noului joc;
		- daca mingea a atins portiunea de manta corespunzatoare portii se apeleaza functia score. Functia score: actualizeaza scorul, mingea i se atribuie portarului echipei care a primit gol si se actualizeaza tabela de scor.
	Mai departe se translateaza mingea cu valorile a si b si se verifica urmatoarea translatie prin apelul functiei path(). Functia path() calculeaza urmatoarea translatie posibila in momentul in care mingea este in miscare. Verifica daca se poate face translatia cu a si b fara coliziuni, in caz contrar actualizeaza valorile
lui a si b. Trateaza coliziunile cu manta si interactiunea cu jucatorii.

	Functia onKey:
	1. Atunci cand mingea se afla in posesia unui jucator: 
		- rotatie in sensul acelor de ceasornic 
		- rotatie inversa acelor de ceasornic
		- lansare minge
	2. In orice moment al jocului:
		- selectia jucatorului anterior sau urmatorului jucator pentru a-l translata
		- translatii jucator: catre dreapta, catre stanga, in sus, in jos pe o arie definita de valorile dx si dy.

Utilizare

	 Programul nu necesita parametri de rulare. Este suficienta rularea aplicatiei "Tema1.exe" pentru pornirea acestuia.

	 Input tastatura
		- tasta 'd' = rotatie a mingei in sensul acelor de ceasornic;
		- tasta 'f' = rotatie a mingei in sens invers acelor de ceasornic;
		- tasta 'm' = schimbarea selectiei la urmatorul jucator;
		- tasta 'n' = schimbarea selectiei la jucatorul anterior;
		- tasta 'l' = translatie dreapta jucator;
		- tasta 'j' = translatie stanga jucator;
		- tasta 'i' = translatie sus jucator;
		- tasta 'k' = translatie jos jucator;
		- tasta 'SPACE' = lansare minge;
		- tasta 'ESC' = iesire program;
