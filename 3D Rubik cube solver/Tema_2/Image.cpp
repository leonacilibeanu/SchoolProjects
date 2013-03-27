#include "Image.h"

 
using namespace std;

Image::Image(char* filename, int imageWidth, int imageHeight)
{
	this->filename = filename;
	this->imageWidth = (GLuint) imageWidth;
	this->imageHeight = (GLuint) imageHeight;
}

Image::~Image(void)
{
}
/*
Functie care incarca textura.
*/
GLuint Image::textureID(){

	GLuint texture;
	unsigned char *data;	//buffer
    FILE *file;				//fisierul in care se gaseste imaginea


	/*
	Se aloca spatiu pentru buffer.
	*/
    data = (unsigned char*) malloc(imageWidth * imageHeight * 3);
 
	/*
	Se deschide fisierul.
	*/
	file = fopen(filename, "rb");
    if (file == NULL) return 0;

    /*
	Se ignora header-ul.
	*/
	fseek(file, 51, SEEK_CUR);

	/*
	Se citeste informatia.
	*/
    fread(data, imageWidth * imageHeight * 3, 1, file);

	/* 
	Fisierul citit este de timp *.bmp => ordinea pixelilor trebuie inversata, in mod normal aceasta
	este BGR.
	Se aduce la forma RGB.
	*/
	char aux;
	for(int bit_pixel = 0; bit_pixel < imageWidth * imageHeight * 3; bit_pixel += 3){

		aux = data[bit_pixel];
		data[bit_pixel] = data[bit_pixel + 2];
		data[bit_pixel + 2] = aux;
		}

    fclose(file);
 
	/*
	Se aloca un  nume texturii.
	*/
    glGenTextures(1, &texture);
 
    glBindTexture(GL_TEXTURE_2D, texture);

	glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
 
    /*
	Se alege modul GL_MODULATE pentru ca textura sa suporte proprietati de iluminare.
	*/
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
 
    /*
	Se creeaza textura.
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
 
    
    free(data);

	return texture;
}
