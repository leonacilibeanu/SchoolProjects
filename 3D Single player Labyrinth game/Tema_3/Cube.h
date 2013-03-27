#pragma once

#include "glut\glut.h"
#include "Image.h"


class Cube
{
public:
	Cube(char* filename, int sizeTex);
	~Cube(void);

	Cube();

	void texture(char* filename, int sizeTex);
	void draw(float size);

public:
	GLuint top, bottom, right, left, front, back;
};
