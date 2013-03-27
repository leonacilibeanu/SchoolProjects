#pragma once
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "dependinte\freeglut.h"

class Face
{
public:
	Face(void);
	~Face(void);

public:
	GLuint color; //textura fetei
	bool sight;	  //sight = true, daca fata este vizibila si false in caz contrar
};
