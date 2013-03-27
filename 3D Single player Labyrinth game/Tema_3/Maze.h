#pragma once
#include "Cube.h"
#include "MathLibrary.h"
#include <vector>
#include <math.h>
#include <stdlib.h>


class Maze
{
public:
	Maze(char* fileGround, char* fileWall, int lG, int lW, float cubeGround, float cubeWall);
	~Maze(void);

	void drawGround(float size);
	void drawWalls(float size);

public:
	float transparency;
	int lengthGround;
	int lengthWall;
	std::vector<Vector3D*> wallsPos;
	Cube* cubeGround;
	Cube* cubeWall;
};
