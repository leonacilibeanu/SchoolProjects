Tema 3 - Labyrinth 


1. Utilizare
	 Programul nu necesita parametri de rulare. Este suficienta rularea aplicatiei "Tema_3.exe" pentru pornirea acestuia.

	1.1 Input tastatura
Taste valabile in timpul rularii jocului:
		- tasta 'w' = misca jucatorul inainte;
		- tasta 's' = misca jucatorul inapoi;
		- tasta 'a' = roteste jucatorul stanga;
		- tasta 'd' = roteste jucatorul dreapta;
		- tasta '1' = selectia camerei Top-Down;
		- tasta '2' = selectia camerei FPS;
		- tasta '3' = selectia camerei TPS;
		- tasta 'ESC' = oprire program;
Taste valabile la terminarea jocului:
		- tasta 'y' = inceperea unui nou joc;
		- tasta 'n' = oprire program(exit);

2. Implementare
	Programul l-am implementat in Visual Studio 2008.

	2.1 Schema generala

		- deschidere aplicatie;
		- interogare date;
		- executia animatiilor fara a interveni utilizatorul
		- primirea unei comenzi de la tastatura;
		- executarea comenzii;
		- asteptarea unei alte comenzi;
		- terminare aplicatie la cererea utilizatorului;

3. Functionalitati
	
	3.1 Standard

		- existenta celor trei camere : Top-Down, FPS, TPS si posibilitatea interschimbarii acestora la orice moment in joc;
		- constructia unui labirint static din cuburi;
		- sfera cu directie de deplasare sugerata de jumatatea neagra a sferei care va fi intotdeauna orientata pe directia de forward a jucatorului;
		- coliziunea jucatorului cu peretii si portalul (folosirea algoritmului de bounding spheres);
		- rotirea directiei de deplasare fata de OY;
		
	3.2 Bonus

		- mapare texturi pentru suprafata de deplasare a jucatorului, peretii labirintului, fortareata, sfera, cubul in care este delimitat spatiul labirintului si cele 3 cuburi care pot fi afisate la sfarsitul jocului(daca jucatorul a castigat, a pierdut, interogarea utilizatorului la pornirea unui nou joc);
		- constructia unei fortarete in centrul labirintului;
		- aparitia a 4 tipuri de inamici(4 culori diferite). Fiecare inamic apare la un moment de timp si porneste pe una din cele 4 axe de iesire din fortareata. Miscarea inamicilor este random;
		- coliziuni jucator cu inamici, bonusuri;
		- coliziune camera cu peretii;
		- coliziuni inamici cu jucator;
		- bonus de invincibilitate(culoare neagra): jucatorul nu mai poate fi omorat de catre inamici pana la sfarsitul jocului;
		- bonus de trecere prin ziduri(culoare gri): la atingerea acestui bonus peretii se fac transparenti si nu se mai tine cont de coliziunea jucatorului cu peretii; inamicii se lovesc in continuare de pereti;
		- animatie portal;
		- aparitia a 3 tipuri de mesaje la terminarea jocului: "you won", "you lost", "play again?".
