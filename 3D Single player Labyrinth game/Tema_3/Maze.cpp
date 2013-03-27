#include "Maze.h"

int walls[20][20] = {{1, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
	{1, 0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  0,  0,  0,  1},
	{1, 0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  0,  0,  0,  1},
	{1,	0,  1,  0,  0,	1,  1,  1,  0,	1,  1,  0,  0,	1,  0,  0,  0,  1,  1,  1},
	{1,	0,	1,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	1},
	{1,	1,	1,	0,	0,	0,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	0,	1,	0,	1},
	{1,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	1,	0,	1},
	{1,	0,	0,	0,	1,	1,	1,	1,	0,	0,	0,	1,	0,	0,	1,	1,	1,	1,	0,	1},
	{1,	1,	1,	0,	1,	0,	0,	1,	0,	0,	0,	1,	1,	1,	1,	0,	0,	1,	0,	1},
	{1,	0,	1,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1},
	{1,	0,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	1},
	{1,	0,	1,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	1},
	{1,	0,	1,	0,	0,	0,	0,	1,	0,	0,	0,	1,	1,	1,	1,	1,	0,	1,	0,	1},
	{1,	0,	1,	0,	1,	1,	1,	1,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	1},
	{1,	0,	1,	0,	0,	0,	0,	1,	0,	0,	0,	1,	0,	1,	1,	1,	0,	0,	0,	1},
	{1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	1,	1,	1,	1,	1},
	{1,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	1},
	{1,	1,	1,	0,	0,	1,	1,	1,	0,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1},
	{1,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	0,	0,	1,	0,	0,	0,	0,	0,	1},
	{1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1}};

Maze::Maze(char* fileGround, char* fileWall, int lG, int lW, float cubeG, float cubeW)
{
	int i, j;
	float x, y, z;

	transparency = false;

	x = (-2.0f)*cubeW;
	y = cubeW+cubeG;
	z = (-2.0f)*cubeW;

	lengthGround = lG;
	lengthWall = lW;
	
	cubeGround = new Cube(fileGround, 256);
	cubeWall = new Cube(fileWall, 256);


	for (i = 0; i < lengthWall; i++){
		for (j = 0; j < lengthWall; j++){

			if (walls[i][j] != 0)
				wallsPos.push_back(new Vector3D(x, y, z));
			z += 2.0f * cubeW;

		}
		z = (-2.0f)*cubeW;
		x += 2.0f * cubeW;
	}

	//std::cout<< "Lungime vector pozitii: " << wallsPos.size();
}

Maze::~Maze(void)
{
}

void Maze::drawGround(float size){
	int i, j;
	float m = 2 * size;

	for(int i = 0; i < lengthGround; i++){
		for(int j = 0; j < lengthGround; j++){
			glPushMatrix();

			glTranslatef(m*(i-11), 0.0f, m*(j-11));
			cubeGround->draw(size);
			glTranslatef(m*(11-i), 0.0f, m*(11-j));
			
			glPopMatrix();
		}
	}
}

void Maze::drawWalls(float size){
	int i,j;
	float m = 2.0f * size;

	if (transparency)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_ONE, GL_ONE);
	}
	else glDisable(GL_BLEND);

	for(int i=0; i < lengthWall ; i++){
		for(int j=0; j < lengthWall ; j++){
			glPushMatrix();
			
			if ( walls[i][j] != 0 ) {
				//glTranslatef(0.0f, -6.0f, -22.0f);
				
				//glTranslatef(-2,2,-2); // primul cub, dreapta jos
				//glTranslatef(0,2,-2); // al doilea cub, dreapta jos, spre stanga
				//glTranslatef(-2,2,0); // al doilea cub, dreapta jos, in sus
				
				// i = 0 => x = -2 
				// i = 1 => x = 0
				// i = 2 => x = 2

				glTranslatef(m*(i-1), 2.0f, m*(j-1));
				cubeWall->draw(size);
				glTranslatef(m*(1-i), -2.0f, m*(1-j));
			}
			
			glPopMatrix();
		}
	}
	glDisable(GL_BLEND);
}
