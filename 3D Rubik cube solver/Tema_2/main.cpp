#include <cstdlib>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>

#include "dependinte\freeglut.h"
#include "Rubik.h"
#include "Step.h"

Rubik* rubik;                  // Cubul Rubik.
std::vector<Step> steps;	   // Vectori de pasi facuti la amestecarea cubului.

GLfloat spin=0.0f;			   // Pasul unghiular de rotatie.
bool transparent = false;	   // Proprietatea de transparenta a cubului.
bool hide = false;			   // hide = true => nu se mai deseneaza cubul
bool shuffle = true;		   // shuffle = true => este permisa amestecarea blocurilor cubului.
int side = 0;				   // Blocul ce urmeaza a fi rotit.
const float sizeCube = 4.5f;   // Dimensiunea laturii cubului Rubik.
float up = 0.0f;
GLfloat rotationX, rotationY;  // Rotatia globala pe X si Y a cubului Rubik.
int win_w = 1 ,win_h = 1;



/*
Functie de reset a ecranului.
*/
void reset(){

	spin = rubik->rotationX = rubik->rotationY = rubik->rotationZ = 0.0f;
	rotationX = rotationY = 0.0f;
	shuffle = true;
	hide = false;
	side = 0;
	up = 0.0f;
	transparent = false;
	steps.clear();
}

/*
Desenarea cubului Rubik.
*/

void drawRubik(float sizeC){

	float size = sizeC / 3.0f;
	float m = size * 2.0f;

	glRotatef(rubik->rotationX, 1, 0, 0);
	glRotatef(rubik->rotationY, 0, 1, 0);
	glRotatef(rubik->rotationZ, 0, 0, 1);

	for (int i = 0; i < 3; i++) {
		glRotatef(rubik->spinX[i], 1, 0, 0);
			for (int j = 0; j < 3; j++) {
				glRotatef(rubik->spinY[j], 0, 1, 0);
				for (int k = 0; k < 3; k++) {
					glRotatef(rubik->spinZ[k], 0, 0, 1);

					glTranslatef(m*(i-1), m*(j-1), m*(k-1));
					rubik->cube[i][j][k]->draw(size);
					glTranslatef(m*(1-i), m*(1-j), m*(1-k));

					glRotatef(-rubik->spinZ[k], 0, 0, 1);
				}
			glRotatef(-rubik->spinY[j], 0, 1, 0);
			}
	glRotatef(-rubik->spinX[i], 1, 0, 0);
	}

	glRotatef(-rubik->rotationZ, 0, 0, 1);
	glRotatef(-rubik->rotationY, 0, 1, 0);
	glRotatef(-rubik->rotationX, 1, 0, 0);
}

/*
Desenarea cubului.
*/

void drawScore(){
	int i,j,k;
	int len = steps.size();
	int pos = 1;

	i = 4;
	j = -4;

	while (len > 0)
	{
		glTranslatef(-4*i, -4*j, 0.0f);
		drawRubik(1.5f);
		glTranslatef(4*i, 4*j, 0.0f);

		pos++;

		if (pos > 9){
			pos = 1;
			i *= (-1);
			j ++;
		}
		else{
			i--;
			if (pos == 6) 
				j *= 1;
		}

		len--;
	}
}

void init()

{
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	glEnable(GL_LIGHT1); 
	glShadeModel(GL_SMOOTH);


	GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);


	
	GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat lightPos[] = {-9.0f, 4.5f, 18.0f, 1.0f};
	GLfloat dir[] = {-9.0f, 4.5f, 18.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);	
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);


    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
	glEnable(GL_NORMALIZE);

    glClearColor(0.5f,0.0f,0.7f,1.0f);

	rotationX = rotationY = 0;

	rubik = new Rubik();

	drawRubik(sizeCube);
	
}



void reshape(int w, int h)
{
	int aspect;
	//transformarea in poarta de vizualizare
	glViewport(0,0, w, h);

	aspect = w / h;

	//se lucreaza pe matricea de proiectie
	glMatrixMode(GL_PROJECTION);
	//se porneste de la matricea identitate
	glLoadIdentity();
	//transformarea de proiectie - specifica si volumul de vizualizare
	gluPerspective(45.0, (GLfloat) aspect, 1.0, 200.0);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -20.0f);

	gluLookAt(0.0f, up, 15.0f,   
		      0.0f, 0.0f, 0.0f,   
			  0.0f, 1.0f, 0.0f);  
	win_w = w;
	win_h = h;


}

//functia de afisare
void display(void)
{
	
	//initializeaza bufferul de culoare
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}
	else 
		glDisable(GL_BLEND);

	glPushMatrix();

	if (!hide)
	{
		glRotatef(rotationX, 1, 0, 0);
		glRotatef(rotationY, 0, 1, 0);
		glRotatef(spin, 1.2f, 0.9f, 0.2f);
	   
		drawRubik(sizeCube);
	}
	else
	{
		drawScore();
	}

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
	
}


/*
Rotatie ascendenta a cubului.
*/
void explode(){

	float rotX, rotY;
	float x, y, z;
	int i,j,k,l;

	rotX = rotY = 0.5;

	for ( i = 0; i < 350; i++){
	
		if ( (i % 25) == 0) 
		{
			rotX += 1.5f;
			rotY += 1.5f;
		}

		glRotatef(rotX, 1.0f, 0.0f, 0.0f);
		glRotatef(rotY, 0.0f, 1.0f, 0.0f);		

		display();
		Sleep(5);
	}
}

/*
Elimina duplicatele din vector.
*/
void removeDuplicate(){
	int i = 0;
	int h = steps.size();

	if (h >= 4)
	while (i < (h - 3)){

		if ((steps[i].axis == steps[i+1].axis) 
			&& (steps[i].angle == steps[i+1].angle) 
			&& (steps[i].side == steps[i+1].side)

			&& (steps[i+1].axis == steps[i+2].axis) 
			&& (steps[i+1].angle == steps[i+2].angle) 
			&& (steps[i+1].side == steps[i+2].side)
			
			&& (steps[i+2].axis == steps[i+3].axis) 
			&& (steps[i+2].angle == steps[i+3].angle) 
			&& (steps[i+2].side == steps[i+3].side))

		{
			steps.erase(steps.begin() + i, steps.begin() + i + 4);
			h = steps.size();
		}
		else 
			i++;
	}
}

/*
Elimina complementele din vector.
*/
void removeComplement(){
	int i = 0;
	int h = steps.size();

	if (h > 1)
	while (i < (h - 1)){
		if ((steps[i].axis == steps[i+1].axis) 
			&& (steps[i].angle == -steps[i+1].angle) 
			&& (steps[i].side == steps[i+1].side))
		{
			steps.erase(steps.begin() + i + 1);
			steps.erase(steps.begin() + i);
			h = steps.size();
		}
		else i++;
	}
}

void solve(){

	int i,j;

	transparent = true;

	removeDuplicate();

	removeComplement();

	if (steps.size() > 0)
	for (i = steps.size()-1; i >= 0; i--){

		switch(steps[i].axis){

			case 'X':
				for (j = 0; j < 30; j++)
				{
					rubik->rotateX(steps[i].side, steps[i].angle);
					Sleep(10);
					display();
				}
				break;

			case 'Y':
				for (j = 0; j < 30; j++)
				{
					rubik->rotateY(steps[i].side, steps[i].angle);
					Sleep(10);
					display();
				}
				break;

			case 'Z':
				for (j = 0; j < 30; j++)
				{
					rubik->rotateZ(steps[i].side, steps[i].angle);
					Sleep(10);
					display();
				}
				break;

			default:
				break;
		}
	}

	transparent = false;

	display();
	Sleep(500);

	explode();

	hide = true;
	up = -15.0f;
	reshape(800,800);
	display();
	Sleep(3000);
}

void animatieDisplay()
{
	spin = spin + 0.2;

	if(spin > 360.0)
		spin = spin -360.0f;

	glutPostRedisplay();
}


void special(int key,int x,int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		rotationX-=5;
		if (rotationX <= -360)
			rotationX += 360;
		break;
	case GLUT_KEY_DOWN:
		rotationX+=5;
		if (rotationX >= 360)
			rotationX -= 360;
		break;
	case GLUT_KEY_LEFT:
		rotationY-=5;
		if (rotationY <= -360)
			rotationY += 360;
		break;
	case GLUT_KEY_RIGHT:
		rotationY+=5;
		if (rotationY >= 360)
			rotationY -= 360;
		break;
	}
	//cere redesenarea scenei
	glutPostRedisplay();
}


void keyboard(unsigned char key,int x,int y)
{
	int i;

	if (shuffle)
	
		switch(key)
		{
		case '1':
			side = 0;
			//select(side);
			break;
		case '2':
			side = 1;
			//select(side);
			break;
		case '3':
			side = 2;
			//select(side);
			break;
		case 'e':
			steps.push_back(Step('X',side,-3.0f));
			for (i = 0; i < 30; i++)
			{
				rubik->rotateX(side,3.0f);
				Sleep(10);
				display();
			}
			break;
		case 'd':
			steps.push_back(Step('X',side,3.0f));
			for ( i = 0; i < 30; i++)
			{
				rubik->rotateX(side,-3.0f);
				Sleep(10);
				display();
			}
			break;
		case 'r':
			steps.push_back(Step('Y',side,-3.0f));
			for ( i = 0; i < 30; i++)
			{
				rubik->rotateY(side,3.0f);
				Sleep(10);
				display();
			}		
			break;
		case 'f':
			steps.push_back(Step('Y',side,3.0f));
			for ( i = 0; i < 30; i++)
			{
				rubik->rotateY(side,-3.0f);
				Sleep(10);
				display();
			}		
			break;
		case 't':
			steps.push_back(Step('Z',side,-3.0f));
			for ( i = 0; i < 30; i++)
			{
				rubik->rotateZ(side,3.0f);
				Sleep(10);
				display();
			}		
			break;

		case 'g':
			steps.push_back(Step('Z',side,3.0f));
			for ( i = 0; i < 30; i++)
			{
				rubik->rotateZ(side,-3.0f);
				Sleep(10);
				display();
			}		
			break;

		case 's':
			solve();
			shuffle = false;
			reset();
			Sleep(3000);
			reshape(800,800);
			display();
			shuffle = true;
			break;
		}

	switch(key){

		case 27:
		exit(0);
	}
	//cere redesenarea scenei
	glutPostRedisplay();
}

//functie apelata la actionarea mouse-ului
void mouse(int buton, int stare, int x, int y)
{
	switch(buton)
	{
	case GLUT_LEFT_BUTTON:
		if(stare == GLUT_DOWN)
			//functia idle animatieDisplay() se apeleaza oricand nu sunt evenimente
			glutIdleFunc(animatieDisplay);
		break;
	case GLUT_RIGHT_BUTTON:
		if(stare== GLUT_DOWN)
			//functia idle este dezactivata 
			glutIdleFunc(NULL);
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Tema 2");

	init();
	
	//animatieDisplay();
	//functia de afisare se numeste display()
	glutDisplayFunc(display);

		
	//functia reshape() este apelata la redimensionarea ferestrei de afisare 
	//sau cand sistemul determina ca s-a acoperit (partial) fereastra
	glutReshapeFunc(reshape);
	
	//functia mouse() se apeleaza la actionarea mouse-ului
	glutMouseFunc(mouse);

	//functia keyboard() se apeleaza la apasarea unei taste
	glutKeyboardFunc(keyboard);

	//functia special() se apeleaza la apasarea unei taste speciale
	glutSpecialFunc(special);



	//contine o bucla in care aplicatia asteapta evenimente
	glutMainLoop();

	//glutTimerFunc(25, animatieDisplay, 0);

	
	return 0;
}