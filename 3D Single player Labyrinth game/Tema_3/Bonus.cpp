#include "Bonus.h"

Bonus::Bonus(void)
{
	show = true;
	rotation = 0.0f;
	this->position = Vector3D(0,0,0);
}

Bonus::~Bonus(void)
{
}

Bonus::Bonus(char *name, Vector3D position){
	this->name = name;

	if (name == "black")
	{
		material[0] = 0.0f;
		material[1] = 0.0f;
		material[2] = 0.0f;
		material[3] = 0.0f;
		material[4] = 0.0f;
	}
	if (name == "grey")
	{
		material[0] = 0.1f;
		material[1] = 0.1f;
		material[2] = 0.1f;
		material[3] = 0.0f;
		material[4] = 0.0f;
	}

	this->position = position;
	show = true;
	rotation = 0.0f;
}

void Bonus::draw(){

	rotation += 2.0f;

	if (rotation >= 360.0f)
		rotation = 0.0f;
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
	glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, material);

	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	glScalef(-0.25f, -0.6f, -0.25f);
	glRotatef(rotation, 0, 1, 0);

	glutSolidIcosahedron();

	glPopMatrix();
}