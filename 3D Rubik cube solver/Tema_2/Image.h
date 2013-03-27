#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>


#include "dependinte\glut.h"
#include "dependinte\freeglut.h"


class Image
{
public:
	Image(void);
	~Image(void);
	Image(char *filename, int imageWidth, int imageHeight);	//constructor imagine

	GLuint textureID();	//incarca textura

public:
	char *filename;
	GLuint imageWidth;
	GLuint imageHeight;
};
