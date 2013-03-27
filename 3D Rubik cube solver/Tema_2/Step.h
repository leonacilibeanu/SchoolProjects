#pragma once

class Step
{
public:
	Step(void);
	~Step(void);
	Step(unsigned char axis, int side, float angle);

public:
	unsigned char axis; // Axa in jurul careia s-a efectuat rotirea.
	int side;			// Blocul care a fost rotit.
	float angle;		// Unghiul complementar unghiului de rotatie.
};
