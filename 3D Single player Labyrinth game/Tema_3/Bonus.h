#pragma once
#include "glut\glut.h"
#include "MathLibrary.h"

class Bonus
{
public:
	Bonus(void);
	Bonus(char* name, Vector3D position);
	~Bonus(void);

	void draw();

public:
	Vector3D position;
	char* name;
	bool show;
	float rotation;
	GLfloat material[5];
};
