#include "Cube.h"

const float scale = 1.0f;

Cube::Cube(void)
{
}

Cube::~Cube(void)
{
}

/*
Constructor Cub. 
Asociaza fiecarei fete a cubului o valoare de adevar, in functie de vizibilitatea acesteia in cadrul intregului cub.
*/

Cube::Cube(bool sightLeft, bool sightBack, bool sightRight, bool sightFront, bool sightTop, bool sightBottom){

	textures();

	left.sight = sightLeft;
	back.sight = sightBack;
	right.sight = sightRight;
	front.sight = sightFront;
	top.sight = sightTop;
	bottom.sight = sightBottom;
}


/*
Functie pentru incarcarea texturilor.
*/
void Cube::textures(){

	GLuint white, red, green, yellow, blue, orange;

	Image *image1 = new Image("red.bmp",256,256);
	red = image1->textureID();
	left.color = red;
	delete image1;

	Image *image2 = new Image("white.bmp",256,256);
	white = image1->textureID();
	bottom.color = white;
	delete image2;

	Image *image3 = new Image("yellow.bmp",256,256);
	yellow = image3->textureID();
	back.color = yellow;
	delete image3;

	Image *image4 = new Image("blue.bmp",256,256);
	blue = image4->textureID();
	front.color = blue;
	delete image4;

	Image *image5 = new Image("orange.bmp",256,256);
	orange = image5->textureID();
	right.color = orange;
	delete image5;

	Image *image6 = new Image("green.bmp",256,256);
	green = image6->textureID();
	top.color = green;
	delete image6;
}

/*
Functie pentru inversarea culorilor a 4 fete primite drept parametrii,
dar si a valorii de adevar a vizibilitatii acestora.
*/

void Cube::switchFaces(Face* face1, Face* face2, Face* face3, Face* face4){
	GLuint aux;
	bool s;

	aux = face1->color;
	face1->color = face2->color;
	face2->color = face3->color;
	face3->color = face4->color;
	face4->color = aux;

	s = face1->sight;
	face1->sight = face2->sight;
	face2->sight = face3->sight;
	face3->sight = face4->sight;
	face4->sight = s;


}

/*
Functie de desenare a cubului.
In functie de vizibilitate, fiecarei fete i se asociaza o textura, daca este la vedere, si culoarea neagra in caz contrar.
@param Lungimea laturii cubului.
*/
void Cube::draw(float size){
	
	GLfloat w[] = {0.65f, 0.65f, 0.65f, 0.65f};	  //luminozitate
	GLfloat b[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; //culoarea fetelor care nu se afla la vedere
	GLfloat s[] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f}; //specular light

	glDisable(GL_COLOR_MATERIAL);
	
    glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialf(GL_FRONT, GL_SHININESS, 60.f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, s);
    

	glPushMatrix();
	
	/*
	Top face.
	*/

	if (top.sight)
	{
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, top.color);

		glBegin(GL_QUADS);

		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, w);
		glNormal3f(0.0, 1.0f, 0.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-size / scale, size / scale, -size / scale);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-size / scale, size / scale, size / scale);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size / scale, size / scale, size / scale);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(size / scale, size / scale, -size / scale); 

		glEnd();
	}
		
	else
	{
		glDisable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);

		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, b);
		glMaterialf(GL_FRONT, GL_SHININESS, 60.f);
		glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, b);		
		glMaterialf(GL_BACK, GL_SHININESS, 60.f);

		glNormal3f(0.0, 1.0f, 0.0f);

		glColor3f(0.0f,0.0f,0.0f);
		glVertex3f(-size / scale, size / scale, -size / scale);
		glVertex3f(-size / scale, size / scale, size / scale);
		glVertex3f(size / scale, size / scale, size / scale);
		glVertex3f(size / scale, size / scale, -size / scale); 

		glEnd();
	}


	/*
	Bottom face.
	*/
  
	if (bottom.sight)
	{
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, bottom.color);

		glBegin(GL_QUADS);

		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, w);
		glNormal3f(0.0, -1.0f, 0.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-size / scale, -size / scale, -size / scale);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(size / scale, -size / scale, -size / scale);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size / scale, -size / scale, size / scale);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-size / scale, -size / scale, size / scale);

		glEnd();
	}
	else
	{
		glDisable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);

		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, b);
		glMaterialf(GL_FRONT, GL_SHININESS, 60.f);
		glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, b);		
		glMaterialf(GL_BACK, GL_SHININESS, 60.f);

		glNormal3f(0.0, -1.0f, 0.0f);

		glColor3f(0.0f,0.0f,0.0f);
		glVertex3f(-size / scale, -size / scale, -size / scale);
		glVertex3f(size / scale, -size / scale, -size / scale);
		glVertex3f(size / scale, -size / scale, size / scale);
		glVertex3f(-size / scale, -size / scale, size / scale);

		glEnd();
	}
	/*
	Left face.
	*/

   if (left.sight)
	{
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, left.color);

		glBegin(GL_QUADS);

		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, w);
		glNormal3f(-1.0, 0.0f, 0.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-size / scale, -size / scale, -size / scale);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-size / scale, -size / scale, size / scale);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-size / scale, size / scale, size / scale);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-size / scale, size / scale, -size / scale); 

		glEnd();
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
		
		glBegin(GL_QUADS);

		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, b);
		glMaterialf(GL_FRONT, GL_SHININESS, 60.f);
		glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, b);		
		glMaterialf(GL_BACK, GL_SHININESS, 60.f);

		glNormal3f(-1.0, 0.0f, 0.0f);

		glColor3f(0.0f,0.0f,0.0f);
		glVertex3f(-size / scale, -size / scale, -size / scale);
		glVertex3f(-size / scale, -size / scale, size / scale);
		glVertex3f(-size / scale, size / scale, size / scale);
		glVertex3f(-size / scale, size / scale, -size / scale); 

		glEnd();
	}
 
   /*
	Right face.
	*/

	if (right.sight)
	{
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, right.color);

		glBegin(GL_QUADS);

		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, w);
		glNormal3f(1.0, 0.0f, 0.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(size / scale, -size / scale, -size / scale);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(size / scale, size / scale, -size / scale);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size / scale, size / scale, size / scale);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(size / scale, -size / scale, size / scale); 

		glEnd();
	}
	else
	{
		glDisable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);

		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, b);
		glMaterialf(GL_FRONT, GL_SHININESS, 60.f);
		glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, b);		
		glMaterialf(GL_BACK, GL_SHININESS, 60.f);

		glNormal3f(1.0, 0.0f, 0.0f);

		glColor3f(0.0f,0.0f,0.0f);

		glVertex3f(size / scale, -size / scale, -size / scale);
		glVertex3f(size / scale, size / scale, -size / scale);
		glVertex3f(size / scale, size / scale, size / scale);
		glVertex3f(size / scale, -size / scale, size / scale); 

		glEnd();
	}
	
	/*
	Front face.
	*/

    if (front.sight)
	{
		glColor3f(1.0f,1.0f,1.0f);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, front.color);

		glBegin(GL_QUADS);

		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, w);
		glNormal3f(0.0, 0.0f, 1.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-size / scale, -size / scale, size / scale);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(size / scale, -size / scale, size / scale);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size / scale, size / scale, size / scale);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-size / scale, size / scale, size / scale); 

		glEnd();
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	
		glBegin(GL_QUADS);

		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, b);
		glMaterialf(GL_FRONT, GL_SHININESS, 60.f);
		glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, b);		
		glMaterialf(GL_BACK, GL_SHININESS, 60.f);

		glNormal3f(0.0, 0.0f, 1.0f);

		glColor3f(0.0f,0.0f,0.0f);

		glVertex3f(-size / scale, -size / scale, size / scale);
		glVertex3f(size / scale, -size / scale, size / scale);
		glVertex3f(size / scale, size / scale, size / scale);
		glVertex3f(-size / scale, size / scale, size / scale); 

		glEnd();
	}
	
    /*
	Back face.
	*/

    if (back.sight)
	{
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, back.color);

		glBegin(GL_QUADS);

		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, w);
		glNormal3f(0.0, 0.0f, -1.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-size / scale, -size / scale, -size / scale);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-size / scale , size / scale, -size / scale);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size / scale, size / scale, -size / scale);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(size / scale, -size / scale, -size / scale); 

		glEnd();

	}
	else
	{
		glDisable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);

		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, b);
		glMaterialf(GL_FRONT, GL_SHININESS, 60.f);
		glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, b);		
		glMaterialf(GL_BACK, GL_SHININESS, 60.f);

		glNormal3f(0.0, 0.0f, -1.0f);

		glColor3f(0.0f,0.0f,0.0f);

		glVertex3f(-size / scale, -size / scale, -size / scale);
		glVertex3f(-size / scale , size / scale, -size / scale);
		glVertex3f(size / scale, size / scale, -size / scale);
		glVertex3f(size / scale, -size / scale, -size / scale); 

		glEnd();
	}

    glDisable(GL_TEXTURE_2D); 

	glPopMatrix();


}

