#include "Cube.h"

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

Cube::Cube(char* filename, int sizeTex){

	texture(filename, sizeTex);

}


/*
Functie pentru incarcarea texturilor.
*/
void Cube::texture(char* filename, int sizeTex){

	GLuint color;

	Image *image = new Image(filename,sizeTex,sizeTex);
	color = image->textureID();

	left = color;
	bottom = color;
	back = color;;
	front = color;
	right = color;
	top = color;

	delete image;
}

/*
Functie de desenare a cubului.
In functie de vizibilitate, fiecarei fete i se asociaza o textura, daca este la vedere, si culoarea neagra in caz contrar.
@param Lungimea laturii cubului.
*/
void Cube::draw(float size){
	
	GLfloat w[] = {0.65f, 0.65f, 0.65f, 0.65f};	  //luminozitate
	GLfloat s[] = {0.8f, 0.8f, 0.8f, 0.8f, 0.8f}; //specular light
	
    glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialf(GL_FRONT, GL_SHININESS, 40.f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, s);
    

	glPushMatrix();
	
	/*
	Top face.
	*/

	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, top);

	glBegin(GL_QUADS);

	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, w);
	//glNormal3f(0.0, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
    glVertex3f( size, size, size);    

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-size, size, size);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-size,-size, size);    

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( size, -size, size);

	glEnd();


	/*
	Bottom face.
	*/

	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bottom);

	glBegin(GL_QUADS);

	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, w);
	glNormal3f( 0.0f,-1.0f, 0.0f);  

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( size,-size,-size);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-size,-size,-size);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-size, size,-size);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( size, size,-size);
	glEnd();
	
	
	/*
	Left face.
	*/

	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, left);

	glBegin(GL_QUADS);

	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, w);
	glNormal3f( 1.0f, 0.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( size, size,-size);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( size, size, size);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( size, -size, size);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( size, -size, -size); 
	glEnd();
	
   /*
	Right face.
	*/

	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, right);

	glBegin(GL_QUADS);

	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, w);
	glNormal3f( 1.0f, 0.0f, 0.0f);

     glTexCoord2f(0.0f, 0.0f);
     glVertex3f(-size, size, size);    

     glTexCoord2f(1.0f, 0.0f);
     glVertex3f(-size, size,-size);    

     glTexCoord2f(1.0f, 1.0f);
     glVertex3f(-size,-size,-size);

     glTexCoord2f(0.0f, 1.0f);
     glVertex3f(-size,-size, size); 


	glEnd();
	
	/*
	Front face.
	*/

	glColor3f(1.0f,1.0f,1.0f);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, front);

	glBegin(GL_QUADS);

	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, w);
	glNormal3f( 0.0f, 0.0f, 1.0f);  

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( size, size,-size);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-size, size,-size);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-size, size, size);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( size, size, size);

	glEnd();
	
    /*
	Back face.
	*/

	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, back);

	glBegin(GL_QUADS);

	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, w);
	glNormal3f( 0.0f, 0.0f,-1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( size,-size, size);    

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-size,-size, size);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-size,-size,-size);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(size,-size,-size);

	glEnd();

    glDisable(GL_TEXTURE_2D); 

	glPopMatrix();

}