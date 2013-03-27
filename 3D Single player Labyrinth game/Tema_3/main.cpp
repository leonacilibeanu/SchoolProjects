#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include "glut\glut.h"
#include "MathLibrary.h"
#include "Bonus.h"

//ground
#include "Maze.h"
int lengthGround = 41;
int lengthWalls = 20;
float cubeGround = 1.0f;
float cubeWall = 1.0f;
Maze* maze;

//camera
#include "Camera.h"
#include "windows.h"
int width = 50;
int height = 50;
Camera camera;
float direction;
float cameraUp = true;
float cameraFPS = false;
float cameraTPS = false;
bool won = false;
bool lost = false;
bool justWon = false;
bool justLost = false;
int globalTime = 0;
float camHeight = 2.0f;

//sky
#include "Image.h"
Cube* sky;
float skyCube = 60.0f;

//reset
Cube* youLost;
Cube* youWon;
Cube* playAgain;
float resetCube = 15.0f;


//portal
float portalRadius = 0.8f;
float portalRotation = 1.0f;
GLUquadricObj *quadric;
Vector3D portalPos;
std::vector<Vector3D> possiblePos;


//player
#include "Player.h"
float PLAYER_RADIUS = 0.3f;
float PLAYER_DIRECTION = 1.f;
float PLAYER_DIRECTION_SPEED = 0.0001f;
Player* player;
bool rotatePlayer = false;
float directionSpeed;
float rotateSpeed = 0.7f;
float invincible = false;

//opponents
const float OPPONENT_RADIUS = 0.3f;
std::vector<Player> opponents;
int opponentType = 0; //0, 1, 2, 3 pentru cele 4 directii; //0 - rosu, 1 - verde, 2- albastru, 3 - galben
std::vector<Vector3D> opponentsDirection;
int frequenceOpponent = 1000;
float opponentSign = 30.0f;

//bonus
std::vector<Bonus*> bonus;
const float BONUS_RADIUS = 0.3f;

//fortareata
Cube* fortress;
float fortressSize = cubeWall;
std::vector<Vector3D*> fortressPos;
GLuint fortressTex;
float fortressTransparency = false;

float angle=0;

void init(){
	int x, y, z;

	GLfloat ambientLight[] = {0.6f, 0.6f, 0.6f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);


    glEnable(GL_DEPTH_TEST);							
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
	glEnable(GL_NORMALIZE);

	/*
	Se incarca texturile.
	*/
	maze = new Maze("grass2.bmp", "wall5.bmp", lengthGround, lengthWalls, cubeGround, cubeWall);
	sky = new Cube("sky1000.bmp", 500);
	fortress = new Cube("fortress.bmp",444);
	youLost = new Cube("youLost.bmp", 500);
	youWon = new Cube("youWon.bmp", 500);
	playAgain = new Cube("playAgain.bmp", 500);

	Image *image = new Image("portal3.bmp",352,351);
	fortressTex = image->textureID();

	/*
	Se creeaza un vector cu pozitiile cuburilor care construiesc fortareata.
	Folosit pentru coliziuni.
	*/

	x = 16.0f; y = 2.0f; z = 14.0f;
	fortressPos.clear();
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 2; j++){
			for (int k = 0; k < 3; k++){

				if (((i == 0) && (j == 0) && (k == 1)) || 
					((i == 1) && (j == 0) && (k == 0)) ||
					((i == 1) && (j == 0) && (k == 1)) ||
					((i == 1) && (j == 0) && (k == 2)) ||
					((i == 2) && (j == 0) && (k == 1))){}
				else
					fortressPos.push_back(new Vector3D(x, y, z));

				z += 2.0f * fortressSize;
			}
			z = 14.0f;
			y += 2.0f * fortressSize;
		}
		z = 14.0f;
		y = 2.0f;
		x += 2.0f * fortressSize;
	}

	fortressTransparency = false;

	player = new Player("player10.bmp", 428, 267);
	player->init(Vector3D(0.0f , 1.5f , 0.5f),
				Vector3D(0.0f, 0.0f, -1.0f),
				Vector3D(0.0f, 1.0f, 0.0f),
				Vector3D(-1.0f, 0.0f, 0.0f), PLAYER_RADIUS);

	invincible = false;

	/*
	Vector cu 4 pozitii posibile ale portalului. 
	La fiecare rulare se alege random una dintre ele.
	*/

	possiblePos.clear();
	possiblePos.push_back(Vector3D(34.0f, 2.0f, 0.0f));
	possiblePos.push_back(Vector3D(34.0f, 2.0f, 34.0f));
	possiblePos.push_back(Vector3D(25.0f, 2.0f, 33.0f));
	possiblePos.push_back(Vector3D(1.0f, 2.0f, 33.0f));

	float p = fmod(rand(), 3.0f) + 1;
	portalPos = possiblePos.at(p);

	opponentType = 0;

	/*
	Vector cu directiile pe care pot pleca inamicii. 
	In functie de opponentType se alege una dintre aceste directii.
	*/
	opponentsDirection.clear();
	opponentsDirection.push_back(Vector3D(-1.0f, 0.0f, 0.0f));
	opponentsDirection.push_back(Vector3D(1.0f, 0.0f, 0.0f));
	opponentsDirection.push_back(Vector3D(0.0f, 0.0f, -1.0f));
	opponentsDirection.push_back(Vector3D(0.0f, 0.0f, 1.0f));	

	opponents.clear();

	Player o;
	o.init(Vector3D(18.0f , 1.5f , 16.0f),
			opponentsDirection.at(opponentType),
			Vector3D(0.0f, 1.0f, 0.0f),
			Vector3D(-1.0f, 0.0f, 0.0f), OPPONENT_RADIUS);
	o.type = opponentType;

	opponents.push_back(o);

	/*
	Vectori de bonusuri.
	Au pozitii statice la fiecare rulare a jocului.
	*/
	bonus.clear();
	bonus.push_back(new Bonus("black", Vector3D(0.0f, 1.7f, 4.0f)));
	bonus.push_back(new Bonus("grey", Vector3D(0.0f, 1.7f, 10.0f)));
	bonus.push_back(new Bonus("black", Vector3D(34.0f, 1.7f, 34.0f)));
	bonus.push_back(new Bonus("grey", Vector3D(34.0f, 1.7f, 28.0f)));
	bonus.push_back(new Bonus("black", Vector3D(34.0f, 1.7f, 0.0f)));
	bonus.push_back(new Bonus("grey", Vector3D(34.0f, 1.7f, 10.0f)));

}

/*
Functia de reset a jocului.
Se reseteaza toate variabilele care au suferit schimbari pe 
parcursul jocului.
*/

void reset(){

	globalTime = 0.0f;

	init();

	camera.init(Vector3D(18 , 55 , 18),
				Vector3D(0.0f, 0.0f, -1.0f),
				Vector3D(-1.0f, 0.0f, 0.0f),
				Vector3D(18,  54, 18));

	cameraUp = true;
	cameraFPS = false;
	cameraTPS = false;

	portalRadius = 0.8f;
	portalRotation = 1.0f;

	rotatePlayer = false;
	rotateSpeed = 0.7f;
}

/*
Functie care testeaza coliziunea dintre doua obiecte aflate in miscare.
*/
bool boundingSphere(Vector3D object1, Vector3D object2, Vector3D object1Speed, Vector3D object2Speed, float object1Radius, float object2Radius){

	Vector3D newObject2 = Vector3D(object2.x + object2Speed.x, object2.y, object2.z + object2Speed.z);
	Vector3D newObject1 = Vector3D(object1.x + object1Speed.x, object1.y, object1.z + object1Speed.z);
	
	float fDistance = object1Radius + object2Radius;
	float fRadius;

	fRadius = sqrt( ((newObject2.x - newObject1.x) * (newObject2.x - newObject1.x)) +
                    ((newObject2.y - newObject1.y) * (newObject2.y - newObject1.y)) +
                    ((newObject2.z - newObject1.z) * (newObject2.z - newObject1.z))  );

    if( fRadius < fDistance )
        return true;

    else
        return false;
}

/*
Functie care verifica daca un punct de afla in interiorul unui cub.
*/

bool intersectsCube(Vector3D object, Vector3D* cubeCenter){
	float scale = 1.3f;

	if ((object.x <= cubeCenter->x + cubeWall * scale) &&
		(object.x >= cubeCenter->x - cubeWall * scale) &&
		(object.y <= cubeCenter->y + cubeWall * scale) &&
		(object.y >= cubeCenter->y - cubeWall * scale) &&
		(object.z <= cubeCenter->z + cubeWall * scale) &&
		(object.z >= cubeCenter->z - cubeWall * scale))

		return true;
	return false;
}

/*
Functie de verificare a coliziunii camerei cu peretii.
*/

bool canMoveCamera(Vector3D object, Vector3D objectSpeed){
	int i;

	if (fortressTransparency)
		return true;
	else
	{

	Vector3D newObject = Vector3D(object.x + objectSpeed.x, object.y + objectSpeed.y, object.z + objectSpeed.z);

	for (i = 0; i < maze->wallsPos.size(); i++)
		if (intersectsCube(newObject, maze->wallsPos[i]))
			return false;

	for (i = 0; i < fortressPos.size(); i++)
		if (intersectsCube(newObject, fortressPos[i]))
			return false;
	}

	return true;
}

/*
Verifica daca obiectul primit ca parametru si descris de pozitie, viteza, raza, se poate
deplasa prin labirint fara a intra in coliziune cu peretii.
Verifica si coliziunile cu bonusurile sau portalul.
*/
bool moveThroughMaze(Vector3D object, Vector3D objectSpeed, float objectRadius)
{
	unsigned int i;
	Vector3D wall;
	float wallRadius = sqrt(1.9f)*cubeWall;

	if (object == player->position)
	{
		if (boundingSphere(portalPos, object, Vector3D(0,0,0), objectSpeed, portalRadius, objectRadius))
			{
				justWon = true;
				return false;
			}

		for (i = 0; i< bonus.size(); i++)
			if (boundingSphere(bonus.at(i)->position, object, Vector3D(0,0,0), Vector3D(0,0,0), BONUS_RADIUS, objectRadius))
			{
				bonus.at(i)->show = false;
				if (bonus.at(i)->name == "grey")
				{
					maze->transparency = true;
					fortressTransparency = true;
				}

				if (bonus.at(i)->name = "black")
					invincible = true;
				}
		if (fortressTransparency)
			return true;
	}
	
	for (i = 0; i < maze->wallsPos.size(); i++)
		{
			wall.x = maze->wallsPos[i]->x;
			wall.y = maze->wallsPos[i]->y;
			wall.z = maze->wallsPos[i]->z;
			if (boundingSphere(wall, object, Vector3D(0,0,0), objectSpeed, objectRadius, wallRadius))
			{
				return false;
			}
		}

		for (i = 0; i < fortressPos.size(); i++)
		{
			wall.x = fortressPos[i]->x;
			wall.y = fortressPos[i]->y;
			wall.z = fortressPos[i]->z;
			if (boundingSphere(wall, object, Vector3D(0,0,0), objectSpeed, objectRadius, wallRadius))
			{
				return false;
			}
		}
	return true;
}

bool moveThroughPlayer(Vector3D object, Vector3D objectSpeed, float objectRadius)
{
	unsigned int i;

	if (object == player->position)
	{
		if (invincible)
			return true;

		for (i = 0; i < opponents.size(); i++)
			if (boundingSphere(opponents.at(i).position, object, Vector3D(0,0,0), Vector3D(0,0,0), OPPONENT_RADIUS, objectRadius))
			{
				justLost = true;
				return false;
			}
	}
	else
	{
		if (boundingSphere(object, player->position, Vector3D(0,0,0), Vector3D(0,0,0), objectRadius, PLAYER_RADIUS))
		{
			if (!invincible)
				justLost = true;
		}
	}

	return true;
}
bool canMove(Vector3D object, Vector3D objectSpeed, float objectRadius){

	if (moveThroughMaze(object, objectSpeed, objectRadius) &&
		moveThroughPlayer(object, objectSpeed, objectRadius))
			return true;
	else
		return false;
}

void drawFortress(){
	float m = fortressSize * 2.0f;

	if (fortressTransparency)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
		glDisable(GL_BLEND);

	glPushMatrix();
	
	glTranslatef(18.0f, 4.0f, 16.0f);

	for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 3; k++) {
					if (((i ==0) && (j == 0) && (k == 1)) || 
						((i == 1) && (j == 0) && (k == 0)) ||
						((i == 1) && (j == 0) && (k == 1)) ||
						((i == 1) && (j == 0) && (k == 2)) ||
						((i == 2) && (j == 0) && (k == 1))){}
					else{

					glTranslatef(m*(i-1), m*(j-1), m*(k-1));
					fortress->draw(fortressSize);
					glTranslatef(m*(1-i), m*(1-j), m*(1-k));
					}
				}
			}
	}

	glPopMatrix();

	glDisable(GL_BLEND);
}
void drawPortal(){

	/*
	Primul inel.
	*/

	GLfloat c1[] = {0.0f, 0.4f, 1.f, 0.0f, 0.0f};
	
   glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c1);
	glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, c1);	

	if (portalRotation == 360.0f)
		portalRotation -= 360.0f;

	glPushMatrix();

	glTranslatef(portalPos.x, portalPos.y, portalPos.z);
	glRotatef(portalRotation, 0, 1, 1);

	glutSolidTorus(0.01f, portalRadius, 40, 40);

	glPopMatrix();

	/*
	Al doilea inel.
	*/

	GLfloat c2[] = {0.0f, 0.8f, 0.2f, 0.0f, 0.0f};

	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c2);
	glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, c2);


	glPushMatrix();

	glTranslatef(portalPos.x, portalPos.y, portalPos.z);
	glRotatef(portalRotation, 1, 0, 1);

	glutSolidTorus(0.01f, portalRadius-0.01, 40, 40);

	glPopMatrix();


	/*
	Al treilea inel.
	*/
	GLfloat c3[] = {0.0f, 0.0f, 1.f, 0.0f, 0.0f};

	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c3);
	glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, c3);


	glPushMatrix();

	glTranslatef(portalPos.x, portalPos.y, portalPos.z);
	glRotatef(portalRotation, 1, 1, 0);

	glutSolidTorus(0.01f, portalRadius-0.02, 40, 40);

	glPopMatrix();



	/*
	Al patrulea inel.
	*/
	glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c1);
	glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, c1);	

	glPushMatrix();

	glTranslatef(portalPos.x, portalPos.y, portalPos.z);
	glRotatef(-portalRotation, 0, 1, 1);

	glutSolidTorus(0.01f, portalRadius-0.03f, 40, 40);

	glPopMatrix();



	/*
	Al cincilea inel.
	*/
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c2);
	glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, c2);	


	glPushMatrix();

	glTranslatef(portalPos.x, portalPos.y, portalPos.z);
	glRotatef(-portalRotation, 1, 0, 1);

	glutSolidTorus(0.01f, portalRadius-0.04, 40, 40);

	glPopMatrix();



	/*
	Al saselea inel.
	*/
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c3);
	glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, c3);	


	glPushMatrix();

	glTranslatef(portalPos.x, portalPos.y, portalPos.z);
	glRotatef(-portalRotation, 1, 1, 0);

	glutSolidTorus(0.01f, portalRadius-0.05, 40, 40);

	glPopMatrix();

	/*
	Cubul.
	*/

	GLfloat c4[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c4);
	glMaterialf(GL_FRONT, GL_SHININESS, 30.f);
	glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, c4);		
	glMaterialf(GL_BACK, GL_SHININESS, 30.f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);


	glPushMatrix();

	glTranslatef(portalPos.x, portalPos.y, portalPos.z);
	glRotatef(45, 0, 0, 1);
	glRotatef(portalRotation, 1, 1, 0);

	glutSolidCube(0.4f);

	glDisable(GL_BLEND);

	glPopMatrix();
	

	portalRotation += 3;

}

void drawBonus(){
	int i;

	for (i = 0; i < bonus.size(); i++)
		if (bonus.at(i)->show)
			bonus.at(i)->draw();
}

void drawOpponents(){
	int i;

	if (globalTime % frequenceOpponent == 0)
		{
			if (opponentType == 4) opponentType = 0;

			Player o;
			o.init(Vector3D(18.0f , 1.5f , 16.0f),
					opponentsDirection.at(opponentType),
					Vector3D(0.0f, 1.0f, 0.0f),
					Vector3D(-1.0f, 0.0f, 0.0f), OPPONENT_RADIUS);
			o.type = opponentType;

			opponents.push_back(o);

			//delete o;
			opponentType++;
			
		}

		if ( opponentSign == 360.0f) 
			opponentSign = 30.0f;

		for (i = 0; i < opponents.size(); i++)
		{
			if (canMove(opponents.at(i).position, opponents.at(i).forward, OPPONENT_RADIUS))
			{
				opponents.at(i).translate_Forward(0.07f, opponents.at(i).forward);
				opponents.at(i).drawOpponent();
				opponentSign++;
			}
			else
			{
				directionSpeed = PLAYER_DIRECTION_SPEED;
				opponents.at(i).speed = Vector3D(0.0f, 0.0f, directionSpeed);
				direction = PLAYER_DIRECTION;
				opponents.at(i).direct(opponentSign * direction, 0.0f, 0.0f);
				opponents.at(i).update();
				opponents.at(i).translate_Forward(0.07f, opponents.at(i).forward);
				opponents.at(i).drawOpponent();
			}
		}
}

//functie afisare
void display(){
	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if ((!justWon) && (!justLost)){
		camera.render();

		sky->draw(skyCube);
		maze->drawGround(cubeGround);
		maze->drawWalls(cubeWall);	

		drawPortal();
		drawFortress();

		if (!cameraFPS)
			player->drawPlayer();

		drawBonus();

		drawOpponents();
	}
	else
	{
		camera.init(Vector3D(20 , 5 , 20),
				Vector3D(0.0f, 0.0f, -1.0f),
				Vector3D(-1.0f, 0.0f, 0.0f),
				Vector3D(20,  4, 20));
		camera.render();

		GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

		GLfloat w[] = {1.f, 1.f, 1.f, 1.f};
		glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, w);

		if (justLost)
		{
			if (lost)
			{
				glPushMatrix();

				glTranslatef(50+2*resetCube*(-1), 2*resetCube*(-1), 50+2*resetCube*(-1));		
				playAgain->draw(resetCube);			
				glTranslatef(50-2*resetCube*(-1), 2*resetCube*1, 50-2*resetCube*(-1));	

				glPopMatrix();
				Sleep(3000);
			}
			else
			{
				glPushMatrix();

				glTranslatef(50+2*resetCube*(-1), 2*resetCube*(-1), 50+2*resetCube*(-1));		
				youLost->draw(resetCube);			
				glTranslatef(50-2*resetCube*(-1), 2*resetCube*1, 50-2*resetCube*(-1));	

				glPopMatrix();

				lost = true;
			}

		}

		if (justWon)
		{
			if (won)
			{
				glPushMatrix();

				glTranslatef(50+2*resetCube*(-1), 2*resetCube*(-1), 50+2*resetCube*(-1));		
				playAgain->draw(resetCube);			
				glTranslatef(50-2*resetCube*(-1), 2*resetCube*1, 50-2*resetCube*(-1));	

				glPopMatrix();
				Sleep(3000);


			}
			else{
				glPushMatrix();

				glTranslatef(50+2*resetCube*(-1), 2*resetCube*(-1), 50+2*resetCube*(-1));		
				youWon->draw(resetCube);			
				glTranslatef(50-2*resetCube*(-1), 2*resetCube*1, 50-2*resetCube*(-1));			

				glPopMatrix();
				won = true;
			}

			
		}

	}

	globalTime++;

	glFlush();

	glutSwapBuffers();
}

void reshape(int width, int height){
	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(float)width/(float)height,0.2,150);
}

void keyboard(unsigned char ch, int x, int y){
	if ((!justWon) && (!justLost))
	{
		switch(ch){
			case 27:
				exit(0);
				break;
			case '1':
				if (!cameraUp)
				{
					camera.init(Vector3D(18 , 55 , 18),
						Vector3D(0.0f, 0.0f, -1.0f),
						Vector3D(-1.0f, 0.0f, 0.0f),
						Vector3D(18,  54, 18));
					cameraUp = true;
					cameraFPS = false;
					cameraTPS = false;
				}
				break;
			case '2':
				if (!cameraFPS)
				{
					camera.init(player->position,
						player->up,
						player->right,
						player->position - player->forward);
					cameraUp = false;
					cameraFPS = true;
					cameraTPS = false;
				}
				break;
			case '3':
				if (!cameraTPS)
				{
					camera.init(Vector3D(player->position.x, camHeight, player->position.z) + player->forward,
						player->up,
						player->right,
						player->position);
					cameraUp = false;
					cameraFPS = false;
					cameraTPS = true;
				}
				break;
			default:
				break;
		}
		if (cameraUp)
			{
				switch(ch){
					case 'w':
						player->translate_Forward(-0.1, player->forward);
						if (!(canMove(player->position,Vector3D(player->forward.x * (-0.1f), 0.0f, player->forward.z * (-0.1f)), PLAYER_RADIUS))
							|| (!canMoveCamera(Vector3D(player->position.x, camHeight, player->position.z) + player->forward, -player->forward)))
							player->translate_Forward(0.1, player->forward);
						break;
					case 's':
						player->translate_Forward(0.1, player->forward);
						if (!(canMove(player->position,Vector3D(player->forward.x * (0.1f), 0.0f, player->forward.z * (0.1f)), PLAYER_RADIUS))
							|| (!canMoveCamera(Vector3D(player->position.x, camHeight, player->position.z) + player->forward, Vector3D(0,0,0))))
							player->translate_Forward(-0.1, player->forward);
						break;
					case 'd':
						directionSpeed = PLAYER_DIRECTION_SPEED;
						player->speed = Vector3D(0.0f, 0.0f, directionSpeed);
						direction = PLAYER_DIRECTION;
						player->direct(direction, 0.0f, 0.0f);
						player->update();
						if (!canMoveCamera(Vector3D(player->position.x, camHeight, player->position.z) + player->forward, Vector3D(0,0,0)))
						{
							player->direct(-direction, 0.0f, 0.0f);
							player->update();
						}
						break;
					case 'a':
						directionSpeed = PLAYER_DIRECTION_SPEED;
						player->speed = Vector3D(0.0f, 0.0f, directionSpeed);
						direction = -PLAYER_DIRECTION;
						player->direct(direction, 0.0f, 0.0f);
						player->update();
						if (!canMoveCamera(Vector3D(player->position.x, camHeight, player->position.z) + player->forward, Vector3D(0,0,0)))
						{
							player->direct(-direction, 0.0f, 0.0f);
							player->update();
						}
						break;
					default:
						break;
				}
			}
		if (cameraFPS)
		{
			switch(ch)
			{
				case 'a':
					directionSpeed = PLAYER_DIRECTION_SPEED;
					player->speed = Vector3D(0.0f, 0.0f, directionSpeed);
					direction = PLAYER_DIRECTION;
					player->direct(direction, 0.0f, 0.0f);
					player->update();
					if (canMoveCamera(Vector3D(player->position.x, camHeight, player->position.z) + player->forward, Vector3D(0,0,0)))
						camera.rotateFPS_OY(-0.018f);
					else
					{
						player->direct(-direction, 0.0f, 0.0f);
						player->update();
					}
					break;
				case 'd':
					directionSpeed = PLAYER_DIRECTION_SPEED;
					player->speed = Vector3D(0.0f, 0.0f, directionSpeed);
					direction = -PLAYER_DIRECTION;
					player->direct(direction, 0.0f, 0.0f);
					player->update();
					if (canMoveCamera(Vector3D(player->position.x, camHeight, player->position.z) + player->forward, Vector3D(0,0,0)))
						camera.rotateFPS_OY(0.018f);
					else
					{
						player->direct(-direction, 0.0f, 0.0f);
						player->update();
					}
					break;
				case 'w':
					player->translate_Forward(-0.1, camera.forward*(-1.0f));
					if ((canMove(player->position, Vector3D(camera.forward.x * (0.1f), 0.0f, camera.forward.z * (0.1f)), PLAYER_RADIUS))
						&& canMoveCamera(Vector3D(player->position.x, camHeight, player->position.z) + player->forward, -player->forward))
					{
						camera.translate_Forward(0.1);
					}
					else
						player->translate_Forward(0.1, camera.forward*(-1.0f));
					break;
				case 's':
					player->translate_Forward(0.1, camera.forward*(-1.0f));
					if ((canMove(player->position, Vector3D(camera.forward.x * (-0.1f), 0.0f, camera.forward.z * (-0.1f)), PLAYER_RADIUS))
						&& canMoveCamera(Vector3D(player->position.x, camHeight, player->position.z) + player->forward, Vector3D(0,0,0)))
					{
						camera.translate_Forward(-0.1);
					}
					else
						player->translate_Forward(-0.1, camera.forward*(-1.0f));
					break;
				default:
					break;
			}

		}
		if (cameraTPS)
		{
			switch(ch)
			{
				case 'w':
					if ((canMove(player->position, Vector3D(camera.forward.x * (0.1f), 0.0f, camera.forward.z * (0.1f)), PLAYER_RADIUS))
						&& canMoveCamera(camera.position, Vector3D(camera.forward.x * (0.1f), 0.0f, camera.forward.z * (0.1f))))
					{
						camera.translate_Forward(0.1);
						player->translate_Forward(-0.1, camera.forward*(-1.0f));
					}
					break;
				case 's':
					if ((canMove(player->position, Vector3D(camera.forward.x * (-0.1f), 0.0f, camera.forward.z * (-0.1f)), PLAYER_RADIUS))
						&& canMoveCamera(camera.position, Vector3D(camera.forward.x * (-0.1f), 0.0f, camera.forward.z * (-0.1f))))
					{
						camera.translate_Forward(-0.1);
						player->translate_Forward(0.1, camera.forward*(-1.0f));
					}
					break;
				case 'a':
					direction = PLAYER_DIRECTION;
					camera.rotateTPS(direction, 0.0f);
					if (canMoveCamera(camera.position, Vector3D(0,0,0)))
					{
						directionSpeed = PLAYER_DIRECTION_SPEED;
						player->speed = Vector3D(0.0f, 0.0f, directionSpeed);
						player->direct(direction, 0.0f, 0.0f);
						player->update();
					}
					else
						camera.rotateTPS(-direction, 0.0f);
					break;
				case 'd':
					direction = -PLAYER_DIRECTION;
					camera.rotateTPS(direction, 0.0f);
					if (canMoveCamera(camera.position, Vector3D(0,0,0)))
					{
						directionSpeed = PLAYER_DIRECTION_SPEED;
						player->speed = Vector3D(0.0f, 0.0f, directionSpeed);
						player->direct(direction, 0.0f, 0.0f);
						player->update();
					}
					else
						camera.rotateTPS(-direction, 0.0f);
					break;
				default:
					break;
			}
		}
	}
	else
	{
		if ((won) || (lost))
		{
			switch(ch){
				case 'y':
					justWon = false;
					justLost = false;
					won = false;
					lost = false;
					reset();
					break;
				case 'n':
					exit(0);
					break;
				default:
					break;
		}
		}
	}

}


//idle
void idle(){
	angle = angle+0.01;
	if(angle >360) angle = angle-360;
	glutPostRedisplay();
}


int main(int argc, char *argv[]){

	//init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);


	//init window
	glutInitWindowSize(900,900);
	glutInitWindowPosition(700,700);
	glutCreateWindow("Tema 3");

	init();

	//callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);


	//z test on
	glEnable(GL_DEPTH_TEST);

	//set background
	glClearColor(0.0,0.5,0.8,1.0);
	
	glutFullScreen();

	glEnable(GL_LINE_SMOOTH);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	glEnable(GL_LIGHT1); 
	glShadeModel(GL_SMOOTH);
	
	GLfloat lightColor0[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat diffuse0[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPos0[] = {17.0f, -20, 17.0f, 1.0};
	GLfloat dir0[] = {0.0f, -1.0, 0.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0);	
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir0);

	GLfloat lightColor1[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat diffuse1[] = {4.0f, 4.0f, 4.0f, 1.0f};
	GLfloat lightPos1[] = {17.0f, 5, 17.0f, 0.0};
	GLfloat dir1[] = {0.0f, -1.0, 0.0f, 1.0f};

	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor1);	
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 300);

	camera.init(Vector3D(18 , 55 , 18),
				Vector3D(0.0f, 0.0f, -1.0f),
				Vector3D(-1.0f, 0.0f, 0.0f),
				Vector3D(18,  54, 18));

	//loop
	glutMainLoop();

	return 0;
}