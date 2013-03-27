 #include "WorldDrawer2d.h"
bool WorldDrawer2d::animation=true;

CoordinateSystem2d *cs1; 
Object2d *scoreTable,*selectedPlayer;	
Ball *ball;								
std::vector<Object2d> players, bar;	//players = vector de obiecte pentru fiecare jucator;  
									//bar = vector de obiecte pentru cifrele afisate in tabela;
std::vector<Point2d> posPlayers;	//posPlayers = vector cu pozitiile initiale ale tuturor jucatorilor; folosit la reset();
float a,b;							//a, b = translatia pe x si y a mingei;
float dx,dy;						//dx, dy = deplasarea maxima pe x si y a unui jucator ; dx*dy = aria de acoperire;
float T = 0.2f;						//T = translatia jucatorilor;
int scoreRed, scoreBlue;
int p = 0;							//p = numarul jucatorului selectat;
float scale = 10.0f;				//scale = variabila folosita pentru scalarea translatiei pe x si y a mingei;
bool justWon = false;

/*
Functia inchide culoarea obiectului trimis ca parametru.
*/
void addShade(Object2d *object){

		if (object->colorx - 0.5f > 0.0f) object->colorx -= 0.5f;
		if (object->colory - 0.5f > 0.0f) object->colory -= 0.5f;
		if (object->colorz - 0.5f > 0.0f) object->colorz -= 0.5f;
}

/*
Functia deschide culoarea obiectului trimis ca parametru.
*/
void removeShade(Object2d *object){

		if (object->colorx > 0.0f) object->colorx += 0.5f;
		if (object->colory > 0.0f) object->colory += 0.5f;
		if (object->colorz > 0.0f) object->colorz += 0.5f;
}

/*
Functia select este folosita pentru selectia jucatorilor.
Primeste parametrul dir, care specifica directia de deplasare a selectiei.
dir = -1, inapoi cu un jucator.
dir = 1, inainte cu un jucator.
*/
void select(int dir){

	if (selectedPlayer != ball->contact)
	removeShade(selectedPlayer);
	p+=dir;
	if (p < 0) p = 21;
	if (p > 21) p = 0;
	selectedPlayer = &players[p];
	addShade(selectedPlayer);
}

void WorldDrawer2d::init(){
	Object2d *o0,*o1,*o2,*o3,*o6,*o7,*o8,*o9,*o10,*o11,*aux;
	unsigned int k;
	cs1 = new CoordinateSystem2d();
	cs_used.push_back(cs1);
	
	/*
	Se creeaza un dreptunghi alb mai  mare decat terenul. Prin suprapunere, se va creea manta terenului. 
	*/
	std::vector<int> topology7;
	std::vector<Point2d> points7;
	points7.push_back(Point2d(-30.2f,19.2f));
	points7.push_back(Point2d(30.2f,19.2f));
	points7.push_back(Point2d(30.2f,-19.2f));
	points7.push_back(Point2d(-30.2f,-19.2f));
	topology7.push_back(0);
	topology7.push_back(1);
	topology7.push_back(2);
	topology7.push_back(2);
	topology7.push_back(3);
	topology7.push_back(0);
	o7 = new Object2d(points7,topology7);
	o7->setcolor(1.0f,1.0f,1.0f);	


	/*
	Terenul propriu-zis.
	*/

	std::vector<Point2d> points6;
	std::vector<int> topology6;
	points6.push_back(Point2d(-30,19));
	points6.push_back(Point2d(30,19));
	points6.push_back(Point2d(30,-19));
	points6.push_back(Point2d(-30,-19));
	topology6.push_back(0);
	topology6.push_back(1);
	topology6.push_back(2);
	topology6.push_back(2);
	topology6.push_back(3);
	topology6.push_back(0);
	o6 = new Object2d(points6,topology6);
	o6->setcolor(0.0f,0.6f,0.0f);


	/*
	Se pastreaza intr-un vector pozitiile ale jucatorilor pentru a putea reveni la starea initiala la momentul resetarii jocului.
	*/

	posPlayers.push_back(Point2d(-28.2f,0.0f));
	posPlayers.push_back(Point2d(-20.0f,-5.0f));
	posPlayers.push_back(Point2d(-20.0f,5.0f));
	posPlayers.push_back(Point2d(-12.0f,-15.0f));
	posPlayers.push_back(Point2d(-12.0f,-5.0f));
	posPlayers.push_back(Point2d(-12.0f,5.0f));
	posPlayers.push_back(Point2d(-12.0f,15.0f));
	posPlayers.push_back(Point2d(-4.0f,-15.0f));
	posPlayers.push_back(Point2d(-4.0f,-5.0f));
	posPlayers.push_back(Point2d(-4.0f,5.0f));
	posPlayers.push_back(Point2d(-4.0f,15.0f));
	posPlayers.push_back(Point2d(4.0f,-15.0f));
	posPlayers.push_back(Point2d(4.0f,-5.0f));
	posPlayers.push_back(Point2d(4.0f,5.0f));
	posPlayers.push_back(Point2d(4.0f,15.0f));
	posPlayers.push_back(Point2d(12.0f,-15.0f));
	posPlayers.push_back(Point2d(12.0f,-5.0f));
	posPlayers.push_back(Point2d(12.0f,5.0f));
	posPlayers.push_back(Point2d(12.0f,15.0f));
	posPlayers.push_back(Point2d(20.0f,-5.0f));
	posPlayers.push_back(Point2d(20.0f,5.0f));
	posPlayers.push_back(Point2d(28.2f,0.0f));



	/* 
	Jucatorii ambelor echipe.
	Se adauga intr-un vector obiectele cu punctele cu coordonatele centrului fiecarui jucator.
	Toate obiectele avand aceeasi raza, nu este nevoie de o topologie explicita, fiind desenate in displayCallbackFunction()
	prin apelarea functiei drawCircle.
	*/

	std::vector<Point2d> points4;
	std::vector<int> topology4;

	points4.push_back(Point2d(-28.2f,0.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(-20.0f,-5.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(-20.0f,5.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(-12.0f,-15.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(-12.0f,-5.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(-12.0f,5.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(-12.0f,15.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(-4.0f,-15.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(-4.0f,-5.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(-4.0f,5.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(-4.0f,15.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(4.0f,-15.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(4.0f,-5.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(4.0f,5.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(4.0f,15.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(12.0f,-15.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(12.0f,-5.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(12.0f,5.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(12.0f,15.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(20.0f,-5.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(20.0f,5.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();
	points4.push_back(Point2d(28.2f,0.0f));
	players.push_back(Object2d(points4,topology4));
	points4.pop_back();

	/*
	Se seteaza culoarea jucatorilor.
	Rosu, albastru, in functie de pozitia in teren a acestora.
	*/

	for (k = 0; k < 22; k++)
	{
		if ((k == 4) || (k ==5) || ((k >= 11) && (k <= 15)) || ((k >=18) && (k <=21)))
			players[k].setcolor(0.0f,0.0f,0.8f);
		else
			players[k].setcolor(0.8f,0.0f,0.0f);
	}


	/*
	Conturul de cerc din centru. Realizat prin suprapunerea a doua cercuri.
	*/

	std::vector<Point2d> points;
	std::vector<int> topology;
	points.push_back(Point2d(0,0));	
	topology.push_back(0);
	o3 = new Object2d(points,topology);
	o3->setcolor(1.0f,1.0f,1.0f);


	std::vector<Point2d> points2;
	std::vector<int> topology2;
	points2.push_back(Point2d(0,0));
	topology2.push_back(0);
	o2 = new Object2d(points2,topology2);
	o2->setcolor(0.0f,0.6f,0.0f);


	/*
	Cercul alb din centru.
	*/

	std::vector<Point2d> points1;
	std::vector<int> topology1;
	points1.push_back(Point2d(0,0));
	topology1.push_back(0);
	o1 = new Object2d(points1,topology1);
	o1->setcolor(1.0f,1.0f,1.0f);

	/*
	Linia de centru.
	*/

	std::vector<Point2d> points0;
	std::vector<int> topology0;
	points0.push_back(Point2d(-0.1f,19.0f));
	points0.push_back(Point2d(0.1f,19.0f));
	points0.push_back(Point2d(0.1f,-19.0f));
	points0.push_back(Point2d(-0.1f,-19.0f));
	topology0.push_back(0);
	topology0.push_back(1);
	topology0.push_back(2);
	topology0.push_back(2);
	topology0.push_back(3);
	topology0.push_back(0);
	o0 = new Object2d(points0,topology0);
	o0->setcolor(1.0f,1.0f,1.0f);

	/*
	Poarta stanga
	*/

	/*
	Pentru ambele porti se merge pe acelasi principiu pe care s-a construit si terenul impreuna cu manta si anume: suprapunerea
	a doua dreptunghiuri.
	*/

	std::vector<Point2d> points8;
	std::vector<int> topology8;
	points8.push_back(Point2d(-30.2f,3.7f));
	points8.push_back(Point2d(-31.7f,3.7f));
	points8.push_back(Point2d(-31.7f,-3.7f));
	points8.push_back(Point2d(-30.2f,-3.7f));
	topology8.push_back(0);
	topology8.push_back(1);
	topology8.push_back(2);
	topology8.push_back(2);
	topology8.push_back(3);
	topology8.push_back(0);
	o8 = new Object2d(points8,topology8);
	o8->setcolor(1.0f,1.0f,1.0f);



	std::vector<Point2d> points9;
	std::vector<int> topology9;
	points9.push_back(Point2d(-30.2f,3.5f));
	points9.push_back(Point2d(-31.5f,3.5f));
	points9.push_back(Point2d(-31.5f,-3.5f));
	points9.push_back(Point2d(-30.2f,-3.5f));
	topology9.push_back(0);
	topology9.push_back(1);
	topology9.push_back(2);
	topology9.push_back(2);
	topology9.push_back(3);
	topology9.push_back(0);
	o9 = new Object2d(points9,topology9);
	o9->setcolor(0.0f,0.0f,0.0f);

	/*
	Poarta dreapta.
	*/

	std::vector<Point2d> points10;
	std::vector<int> topology10;
	points10.push_back(Point2d(30.2f,3.5f));
	points10.push_back(Point2d(31.5f,3.5f));
	points10.push_back(Point2d(31.5f,-3.5f));
	points10.push_back(Point2d(30.2f,-3.5f));
	topology10.push_back(0);
	topology10.push_back(1);
	topology10.push_back(2);
	topology10.push_back(2);
	topology10.push_back(3);
	topology10.push_back(0);
	o10 = new Object2d(points10,topology10);
	o10->setcolor(0.0f,0.0f,0.0f);


	std::vector<Point2d> points11;
	std::vector<int> topology11;
	points11.push_back(Point2d(30.2f,3.7f));
	points11.push_back(Point2d(31.7f,3.7f));
	points11.push_back(Point2d(31.7f,-3.7f));
	points11.push_back(Point2d(30.2f,-3.7f));
	topology11.push_back(0);
	topology11.push_back(1);
	topology11.push_back(2);
	topology11.push_back(2);
	topology11.push_back(3);
	topology11.push_back(0);
	o11 = new Object2d(points11,topology11);
	o11->setcolor(1.0f,1.0f,1.0f);


	/* Scor */

	/*
	Tabela scor
	*/

	std::vector<Point2d> points12;
	std::vector<int> topology12;
	points12.push_back(Point2d(30.6f,19.1f));
	points12.push_back(Point2d(38.2f,19.1f));
	points12.push_back(Point2d(38.2f,14.1f));
	points12.push_back(Point2d(30.6f,14.1f));
	points12.push_back(Point2d(34.4f,19.1f));
	points12.push_back(Point2d(34.4f,14.1f));
	topology12.push_back(0);
	topology12.push_back(1);
	topology12.push_back(1);
	topology12.push_back(2);
	topology12.push_back(2);
	topology12.push_back(3);
	topology12.push_back(3);
	topology12.push_back(0);
	topology12.push_back(4);
	topology12.push_back(5);
	scoreTable = new Object2d(points12,topology12);
	scoreTable->setcolor(1.0f,1.0f,1.0f);

	/*
	Se adauga in vectorul bar, obiectele specifice fiecarei schimbari de scor. 
	Primele 4 obiecte pentru echipa Red, urmatoarele 4 pentru echipa Blue.
	*/

	/*
	Scor echipa Red.
	*/

	std::vector<Point2d> points13;
	std::vector<int> topology13;
	points13.push_back(Point2d(31.3f,18.4f));
	points13.push_back(Point2d(33.7f,18.4f));
	points13.push_back(Point2d(33.7f,14.9f));
	points13.push_back(Point2d(31.3f,14.9f));
	topology13.push_back(0);
	topology13.push_back(1);
	topology13.push_back(1);
	topology13.push_back(2);
	topology13.push_back(2);
	topology13.push_back(3);
	topology13.push_back(3);
	topology13.push_back(0);
	bar.push_back(Object2d(points13, topology13));
	points13.clear(); topology13.clear();
	points13.push_back(Point2d(32.5f,18.6f));
	points13.push_back(Point2d(32.5f,14.7f));
	topology13.push_back(0);
	topology13.push_back(1);
	bar.push_back(Object2d(points13, topology13));
	points13.clear(); topology13.clear();
	points13.push_back(Point2d(31.3f,18.4f));
	points13.push_back(Point2d(33.7f,18.4f));
	points13.push_back(Point2d(33.7f,16.65f));
	points13.push_back(Point2d(31.3f,16.65f));
	points13.push_back(Point2d(31.3f,14.9f));
	points13.push_back(Point2d(33.7f,14.9f));
	topology13.push_back(0);
	topology13.push_back(1);
	topology13.push_back(1);
	topology13.push_back(2);
	topology13.push_back(2);
	topology13.push_back(3);
	topology13.push_back(3);
	topology13.push_back(4);
	topology13.push_back(4);
	topology13.push_back(5);
	bar.push_back(Object2d(points13, topology13));
	points13.clear(); topology13.clear();
	points13.push_back(Point2d(31.3f,18.4f));
	points13.push_back(Point2d(33.7f,18.4f));
	points13.push_back(Point2d(33.7f,16.65f));
	points13.push_back(Point2d(33.7f,14.9f));
	points13.push_back(Point2d(31.3f,14.9f));
	points13.push_back(Point2d(33.7f,16.65f));
	points13.push_back(Point2d(32.0f,16.65f));
	topology13.push_back(0);
	topology13.push_back(1);
	topology13.push_back(1);
	topology13.push_back(2);
	topology13.push_back(2);
	topology13.push_back(3);
	topology13.push_back(3);
	topology13.push_back(4);
	topology13.push_back(5);
	topology13.push_back(6);
	bar.push_back(Object2d(points13, topology13));
	points13.clear(); topology13.clear();

	for (k = 0; k < 4; k++)
		bar[k].setcolor(0.8f,0.0f,0.0f);
	
	/*
	Scor echipa Blue
	*/

	points13.push_back(Point2d(35.1f,18.4f));
	points13.push_back(Point2d(37.5f,18.4f));
	points13.push_back(Point2d(37.5f,14.9f));
	points13.push_back(Point2d(35.1f,14.9f));
	topology13.push_back(0);
	topology13.push_back(1);
	topology13.push_back(1);
	topology13.push_back(2);
	topology13.push_back(2);
	topology13.push_back(3);
	topology13.push_back(3);
	topology13.push_back(0);
	bar.push_back(Object2d(points13, topology13));
	points13.clear(); topology13.clear();
	points13.push_back(Point2d(36.3f,18.6f));
	points13.push_back(Point2d(36.3f,14.7f));
	topology13.push_back(0);
	topology13.push_back(1);
	bar.push_back(Object2d(points13, topology13));
	points13.clear(); topology13.clear();
	points13.push_back(Point2d(35.1f,18.4f));
	points13.push_back(Point2d(37.5f,18.4f));
	points13.push_back(Point2d(37.5f,16.65f));
	points13.push_back(Point2d(35.1f,16.65f));
	points13.push_back(Point2d(35.1f,14.9f));
	points13.push_back(Point2d(37.5f,14.9f));
	topology13.push_back(0);
	topology13.push_back(1);
	topology13.push_back(1);
	topology13.push_back(2);
	topology13.push_back(2);
	topology13.push_back(3);
	topology13.push_back(3);
	topology13.push_back(4);
	topology13.push_back(4);
	topology13.push_back(5);
	bar.push_back(Object2d(points13, topology13));
	points13.clear(); topology13.clear();
	points13.push_back(Point2d(35.1f,18.4f));
	points13.push_back(Point2d(37.5f,18.4f));
	points13.push_back(Point2d(37.5f,16.65f));
	points13.push_back(Point2d(37.5f,14.9f));
	points13.push_back(Point2d(35.1f,14.9f));
	points13.push_back(Point2d(37.5f,16.65f));
	points13.push_back(Point2d(35.8f,16.65f));
	topology13.push_back(0);
	topology13.push_back(1);
	topology13.push_back(1);
	topology13.push_back(2);
	topology13.push_back(2);
	topology13.push_back(3);
	topology13.push_back(3);
	topology13.push_back(4);
	topology13.push_back(5);
	topology13.push_back(6);
	bar.push_back(Object2d(points13, topology13));
	points13.clear(); topology13.clear();

	for (k = 4; k < bar.size(); k++)
		bar[k].setcolor(0.0f,0.0f,0.8f);

	
	/*
	Mingea de fotbal.
	Se seteaza coordonatele initiale si jucatorul de contact, care in acest caz va fi portarul echipei Blue.
	*/

	std::vector<Point2d> points_aux;
	std::vector<int> topology_aux;
	points_aux.push_back(Point2d(27.0f,0.0f));
	topology_aux.push_back(0);
	aux = new Object2d(points_aux,topology_aux);
	aux->setcolor(0.0f,0.0f,0.0f);
	ball = new Ball(aux);
	ball->setContact(&players[players.size() - 1]);


	/*
	Toate obiectele mai sus create se adauga la sistemul de coordonate cs1, in ordine inversa fata de cum vor aparea pe ecran.
	*/

	cs1->objectAdd(ball->object);	// minge de fotbal
	cs1->objectAdd(o0);				//linia de mijloc

	for ( k = 0; k < 22; k++)
	cs1->objectAdd(&players[k]);	// jucatori 

	cs1->objectAdd(o1);				//cercul alb din centru
	cs1->objectAdd(o2);				//cercul verde
	cs1->objectAdd(o3);				//cercul mare alb
	cs1->objectAdd(o6);				//dreptunghi verde = teren
	cs1->objectAdd(o7);				//dreptunghi alb
	cs1->objectAdd(o9);				//porti
	cs1->objectAdd(o8);
	cs1->objectAdd(o10);
	cs1->objectAdd(o11);
	cs1->objectAdd(scoreTable);		//tabela de scor
	cs1->objectAdd(&bar[0]);		//scorul 0 pentru echipa Red
	cs1->objectAdd(&bar[4]);		//scorul 0 pentru echipa Blue

	selectedPlayer = &players[p];	//pozitia initiala a selectiei = portar echipa Red
	addShade(selectedPlayer);
}

/*
Functia reset se apeleaza la resetarea jocului in momentul in care una din echipe a dat 3 goluri.
Reinitializeaza scorul ambelor echipe la 0, inlocuieste in tabela obiectele existente cu cele
echivalente cu 0, seteaza pozitia initiala de selectie, muta jucatorii in pozitia initiala de plecare, 
alege random un portar care se va afla in posesia mingei la inceperea noului joc.
*/

void reset(){
	int j;
	float p1,p2;
	
	/*
	Reset scor.
	*/
	scoreRed = 0;
	scoreBlue = 0;
	p = 0;

	/*
	Reset pozitii jucatori.
	*/
	for ( j = 0; j < 22; j++)
	{
		p1 = posPlayers[j].x;
		p2 = posPlayers[j].y;
		players[j].points[0].set(p1,p2);
	}

	/*
	Reset pozitie initiala de selectie.
	*/
	selectedPlayer = &players[p];
	addShade(selectedPlayer);

	/*
	Alege random un portar care va fi jucatorul de contact al mingei.
	*/
	if ((rand()%2)-1 == 0)
	{
		p1 = players[players.size() - 1].points[0].x - 1.2f;
		p2 = players[players.size() - 1].points[0].y;
		ball->contact = &players[players.size() - 1];

	}
	else
	{
		p1 = players[0].points[0].x + 1.2f;
		p2 = players[0].points[0].y;
		ball->contact = &players[0];
	}

	/*
	Reset pozitie minge la portar.
	*/
	ball->object->points[0].set(p1,p2);
	
	/*
	Schimba culoarea jucatorului cu care mingea se afla in contact.
	*/
	ball->darkenedContact = true;
	addShade(ball->contact);

	/*
	Sterge obiectele aflate in tabela si le inlocuieste cu cele corespunzatoare cu 0.
	*/
	j = cs1->objects.size() - 1;
	cs1->objectRemove(cs1->objects[j]);
	cs1->objectRemove(cs1->objects[j-1]);
	cs1->objectAdd(&bar[0]);
	cs1->objectAdd(&bar[4]);
	
}


/*
Functia score este apelata in momentul in care una din echipe inscrie un gol.
Parametrul p este folosit pentru identificarea echipei care a dat gol.
p = 1, pentru echipa Blue; p = -1, pentru echipa Red.
Se actualizeaza scorul, mingea i se atribuie portarului echipei care a primit gol si se actualizeaza tabela de scor.
*/

void score(float p){
	Object2d *goalKeeper;

	a = 0;
	b = 0;
	ball->foundPlayer = true;
	
	if ( p == 1.0f) 
	{
		scoreBlue++;
		goalKeeper = &players[0];
		ball->setContact(goalKeeper);
		ball->object->points[0].set(goalKeeper->points[0].x + 1.2f, goalKeeper->points[0].y);
		cs1->objectRemove(&bar[scoreBlue + 3]);
		cs1->objectAdd(&bar[scoreBlue + 4]);
	}
	else
	{
		scoreRed++;
		goalKeeper = &players[players.size() -1];
		ball->setContact(goalKeeper);
		ball->object->points[0].set(goalKeeper->points[0].x - 1.2f, goalKeeper->points[0].y);
		cs1->objectRemove(&bar[scoreRed-1]);
		cs1->objectAdd(&bar[scoreRed]);
	}
}

/*
Functie de calcul a distantei dintre 2 puncte.
*/

float dist(float x1, float y1, float x2, float y2){
	float d = (sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
	return d;
}

/*
Functia path calculeaza urmatoarea translatie posibila in momentul in care mingea este in miscare.
Verifica daca se poate face translatia cu a si b fara coliziuni, in caz contrar actualizeaza valorile
lui a si b.
Trateaza coliziunile cu manta si interactiunea cu jucatorii.
*/

void path(){

	float p1 = a;
	float p2 = b;
	float x = ball->object->points[0].x;
	float y = ball->object->points[0].y;
	bool ok = true; // verifica daca mingea intra in coliziune cu manta
	unsigned int k = 0;
	float d = 0.0f;

	/*
	Daca mingea se afla in contact cu un jucator ramane pe loc, translatiile pe x si y vor fi ambele 0.
	*/
	if (ball->foundPlayer) 
	{ 
		a = 0.0f; 
		b = 0.0f;
	}
	else
	{
		d = dist(a,b,0.0f,0.0f);

		/*
		Se verifica daca se afla un jucator in imediata apropiere a mingei.
		Daca mingea interactioneaza cu un jucator: se calculeaza translatia necesara pana la contactul
		cu jucatorul, se actualizeaza variabila foundPlayer si se actualizeaza campul contact al mingei.
		*/

		while ((k < 22) && (ball->foundPlayer == false))
		{
			if ((dist(x, y, players[k].points[0].x, players[k].points[0].y)/scale - 0.005f) < d ) {
				ball->foundPlayer = true;
				ball->setContact(&players[k]);
				if (a != 0)
					a = (players[k].points[0].x - x - (a/abs(a))*(1.2f))/scale;
				if (b != 0)
					b = (players[k].points[0].y - y - (b/abs(b))*(1.2f))/scale;
			}
			k++;
		}

		/*
		Daca mingea nu interactioneaza cu un jucator se verifica eventualele coliziuni cu manta.
		*/
		if (!ball->foundPlayer) {

			/*
			Se verfica daca mingea a lovit deja manta si se schimba directia de translatie pe x sau y.
			*/

			if (( x == 29.5f ) || ( x == -29.5f)) {
				a = ball->l1; 
				b = ball->l2;
				a *= -1.0f; 
				ok = false;
			}
		
			if (( y == 18.5f ) || ( y == -18.5f)) {
				if (ok) 
					a = ball->l1; 
				b = ball->l2; 
				b *= -1.0f; 
				ok = false;
			}

			/*
			Se verifica daca la urmatorul pas mingea va lovi manta cu o translatie mai mica decat cea
			stabilita si actualizeaza valoarea cu o valoare convenabila.
			*/
			if (ok == true) 
			{
				x += a;
				y += b;

				if ( x > 29.5f ) 
				{ 
					a = 29.5f - ( x - a);
					ok = false;
				}
				else 
					if ( x < -29.5f) 
					{ 
						a = -29.5f - ( x - a); 
						ok =false;
					}

				if ( y > 18.5f ) 
				{ 
					b = 18.5f - (y - b); 
					ok = false;
				}
				else 
					if ( y < -18.5f) 
					{ 
						b = -18.5f - (y - b) ; 
						ok = false;
					}

			/*
			Se actualizeaza valoarea penultimei translatii.
			*/
			if (!ok) 
				ball->lastTranslation(p1,p2);
			}
		}
	}
}

/*
In functia onIdle se verifica starea jocului pentru fiecare frame.
*/
void WorldDrawer2d::onIdle(){

	float x = ball->object->points[0].x;
	float y = ball->object->points[0].y;
	unsigned int i, n;
	
	n = cs1->objects.size();

	/*
	Daca mingea a intrat in contact cu un jucator, se va schimba culoarea acestuia, la fel si daca mingea pleaca de la un jucator,
	culoarea acestuia va reveni la culoarea initiala.
	Se folosesc functiile addShade si removeShade pentru aceste schimbari.
	Variabila darkenedContact asigura o singura modificare a culorii pentru fiecare stare a jucatorului:
	in cazul contactului cu mingea si a pasarii acesteia.
	*/

	if (ball->foundPlayer)
	{
		if (!ball->darkenedContact) {
			ball->darkenedContact = true;
			addShade(ball->contact);
		}
	}
	else
	{
		if (ball->darkenedContact) {
			 ball->darkenedContact = false;
			 if (ball->contact != selectedPlayer)
				 removeShade(ball->contact);
		 }
	 }

	/*
	Daca una din echipe a inscris 3 goluri pe frame-ul anterior, se pastreaza culoarea ecranului timp de 3 secunde,
	dupa care acesta revine la culorile initiale, se apeleaza functia reset si se modifica valoarea de adevar a
	variabilei justWon.
	*/
	if (justWon) 
	{
		Sleep(3000);
		for ( i = 0; i < n; i++)
			removeShade(cs1->objects[i]);
		reset();
		justWon = false;
	}


	/*
	Daca au fost inscrise 3 goluri se inchide culoarea ecranului prin modificarea culorii fiecarui obiect
	si se actualizeaza valoarea de adevar a variabilei justWon.
	*/

	if ((scoreRed == 3) || (scoreBlue == 3))
	{
		for ( i = 0; i < n; i++)
			addShade(cs1->objects[i]);
		justWon = true;
	}

	/*
	Daca mingea a atins portiunea de manta corespunzatoare portii se apeleaza functia score, cu parametrul corespunzator
	echipei care a marcat.
	*/
	if (!ball->foundPlayer)
	if ((y >= -3.0f) && (y <= 3.0f))
	{
		if (x == -29.5f) 
			score(1.0f);
	
		if (x == 29.5f) 
			score(-1.0f);
	}

	/*
	Se translateaza mingea cu a si b.
	*/
	ball->object->translate(a,b);

	/*
	Se verifica urmatoarea translatie posibila.
	*/
	path();
}


void WorldDrawer2d::onKey(unsigned char key){
	float xp = posPlayers[p].x;
	float yp = posPlayers[p].y;

	if (ball->foundPlayer)
	switch(key){
		/*
		Rotatie in sensul acelor de ceasornic.
		*/
		case 'd':
			{ 
				ball->object->rotateRelativeToPoint(ball->contact->points[0],0.08f);
				break;
			}
		/*
		Rotatie inversa acelor de casornic.
		*/
		case 'f':
				{ 
					ball->object->rotateRelativeToPoint(ball->contact->points[0],-0.08f);
				break;
			}
		/*
		Lansarea mingei cu o translatie egala cu diferenta dintre coordonatele centrului mingei
		si coordonatele centrului jucatorului.
		*/
		case KEY_SPACE:
			{
				a = (ball->object->points[0].x - ball->contact->points[0].x)/scale;
				b = (ball->object->points[0].y - ball->contact->points[0].y)/scale;
				ball->foundPlayer = false;
				break;
			}
		default:
			break;
	}
	
	switch(key){
		/*
		Schimbarea selectiei la urmatorul jucator
		*/
		case 'm': 
			{
				select(1);
				break;
			}
		/*
		Schimbarea selectiei la jucatorul anterior
		*/
		case 'n':
			{
				select(-1);
				break;
			}
		/*
		Translatie dreapta jucator
		*/
		case 'l':
			{
				if ((selectedPlayer->points[0].x + T) <= (xp + dx)) 
					if ((selectedPlayer != &players[21]) || ((selectedPlayer == &players[21]) && (selectedPlayer->points[0].x != xp)))
					{
						if ((selectedPlayer == ball->contact) && (ball->foundPlayer)) ball->object->translate(T,0.0f);
						selectedPlayer->translate(T,0.0f);
					}
				break;
			}
		/*
		Translatie stanga jucator
		*/
		case 'j':
			{
				if ((selectedPlayer->points[0].x - T) >= (xp - dx)) 
					if ((selectedPlayer != &players[0]) || ((selectedPlayer == &players[0]) && (selectedPlayer->points[0].x != xp)))
					{
						if ((selectedPlayer == ball->contact) && (ball->foundPlayer)) ball->object->translate((-1.0f)*T,0.0f);
						selectedPlayer->translate((-1.0f)*T,0.0f);
					}
				break;
			}
		/*
		Translatie sus jucator
		*/
		case 'i':
			{
				if ((selectedPlayer->points[0].y + T) <= (yp + dy))
				{
					if ((selectedPlayer == ball->contact) && (ball->foundPlayer)) ball->object->translate(0.0f,T);
					selectedPlayer->translate(0.0f,T);
				}
				break;
			}
		/*
		Translatie jos jucator
		*/
		case 'k':
			{
				if ((selectedPlayer->points[0].y - T) >= (yp - dy))
				{
					if ((selectedPlayer == ball->contact) && (ball->foundPlayer)) ball->object->translate(0.0f,(-1.0f)*T);
					selectedPlayer->translate(0.0f,(-1.0f)*T);
				}
				break;
			}
		default:
			break;
	}
}


int main(int argc, char** argv){
	WorldDrawer2d wd2d(argc,argv,1200,700,200,100,std::string("Tema1"));
	
	/*
	Se seteaza valorile initiale ale translatiei, scorul si deplasarea maxima a unui jucator.
	*/
	a = 0.0f;
	b = 0.0f;
	scoreRed = 0;
	scoreBlue = 0;
	dx = 3.3f;
	dy = 4.3f;

	wd2d.init();
	wd2d.run();
	return 0;
}