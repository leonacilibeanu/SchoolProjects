#pragma once

#include "dependinte\freeglut.h"
#include "Image.h"
#include "Face.h"


class Cube
{
public:
	Cube(void);
	~Cube(void);

	Cube(bool sightLeft, bool sightBack, bool sightRight, bool sightFront, bool sightTop, bool sightBottom);

	void textures();
	void switchFaces(Face* face1,Face* face2,Face* face3,Face* face4);
	void draw(float size);

public:
	Face top, bottom, right, left, front, back;
};
