#pragma once
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "Cube.h"

class Rubik
{
public:
	Rubik(void);
	~Rubik(void);

	void rotateX(int side, float angle); // Rotatie pe axa X a unui bloc dat
	void rotateY(int side, float angle); // Rotatie pe axa Y
	void rotateZ(int side, float angle); // Rotatie pe axa Z

public:

	Cube* cube[3][3][3];				 // Matrice repezentand cubul Rubik.
	float spinX[3], spinY[3], spinZ[3];  // Valoarea unghiului de rotatie pe fiecare axa, a fiecarui bloc.
	float rotationX, rotationY, rotationZ; // Valoarea unghiului de rotatie al cubului Rubik pe fiecare axa.

};
