#include "Rubik.h"

/*
Constructor cub Rubik.
Initial, valoarea unghiului de rotatie pe cele 3 axe este 0, la fel si unghiurile de rotatie globale.
Se creeaza 27 de cuburi, in functie de vizibilitatea fetelor.
*/
Rubik::Rubik(void)
{
	spinX[0] = spinX[1] = spinX[2] = 0;
	spinY[0] = spinY[1] = spinY[2] = 0;
	spinZ[0] = spinZ[1] = spinZ[2] = 0;

	rotationX = rotationY = rotationZ = 0;

	cube[0][0][0] = new Cube(true, true, false, false, false, true);
	cube[0][1][0] = new Cube(true, true, false, false, false, false);
	cube[0][2][0] = new Cube(true, true, false, false, true, false);

	cube[0][0][1] = new Cube(true, false, false, false, false, true);
	cube[0][1][1] = new Cube(true, false, false, false, false, false);
	cube[0][2][1] = new Cube(true, false, false, false, true, false);

	cube[0][0][2] = new Cube(true, false, false, true, false, true);
	cube[0][1][2] = new Cube(true, false, false, true, false, false);
	cube[0][2][2] = new Cube(true, false, false, true, true, false);

	cube[1][0][0] = new Cube(false, true, false, false, false, true);
	cube[1][1][0] = new Cube(false, true, false, false, false, false);
	cube[1][2][0] = new Cube(false, true, false, false, true, false);

	cube[1][0][1] = new Cube(false, false, false, false, false, true);
	cube[1][1][1] = new Cube(false, false, false, false, false, false);
	cube[1][2][1] = new Cube(false, false, false, false, true, false);

	cube[1][0][2] = new Cube(false, false, false, true, false, true);
	cube[1][1][2] = new Cube(false, false, false, true, false, false);
	cube[1][2][2] = new Cube(false, false, false, true, true, false);

	cube[2][0][0] = new Cube(false, true, true, false, false, true);
	cube[2][1][0] = new Cube(false, true, true, false, false, false);
	cube[2][2][0] = new Cube(false, true, true, false, true, false);

	cube[2][0][1] = new Cube(false, false, true, false, false, true);
	cube[2][1][1] = new Cube(false, false, true, false, false, false);
	cube[2][2][1] = new Cube(false, false, true, false, true, false);

	cube[2][0][2] = new Cube(false, false, true, true, false, true);
	cube[2][1][2] = new Cube(false, false, true, true, false, false);
	cube[2][2][2] = new Cube(false, false, true, true, true, false);
		
}

Rubik::~Rubik(void)
{
}

/*
Rotatie pe X.
@param Blocul ce urmeaza a fi rotit.
@param Unghiul de rotatie.
*/

void Rubik::rotateX(int side, float angle){

	/*
	Se verifica daca rotatia pe Y si Z este nula.
	Doar in acest caz se poate efectua rotatia pe X a blocului dat.
	*/

	if ((fmod((double)spinY[0],90.0) == fmod((double)spinY[1],90.0)) 
		&& (fmod((double)spinY[1],90.0) == fmod((double)spinY[2],90.0)) 
		&& (fmod((double)spinY[2],90.0) == 0.0 )
		&& (fmod((double)spinZ[0],90.0) == fmod((double)spinZ[1],90.0)) 
		&& (fmod((double)spinZ[1],90.0) == fmod((double)spinZ[2],90.0)) 
		&& (fmod((double)spinZ[2],90.0) == 0.0 ))
	{

		spinX[side] += angle;

		/*
		Se verifica daca s-a efectuat o rotatie completa. 
		In caz afirmativ, se anuleaza rotatia pe X, se rearanjeaza cuburile si se inverseaza fetele acestora.
		*/
		if ((angle < 0.0f) && (spinX[side] == -90.0f))
		{
			spinX[side] = 0;
    
			Cube *aux = cube[side][0][0];
			cube[side][0][0] = cube[side][2][0];
			cube[side][2][0] = cube[side][2][2];
			cube[side][2][2] = cube[side][0][2];
			cube[side][0][2] = aux;
    
			aux = cube[side][1][0];
			cube[side][1][0] = cube[side][2][1];
			cube[side][2][1] = cube[side][1][2];
			cube[side][1][2] = cube[side][0][1];
			cube[side][0][1] = aux;

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3 ; j++)
					cube[side][i][j]->switchFaces(&cube[side][i][j]->top,&cube[side][i][j]->front,&cube[side][i][j]->bottom,&cube[side][i][j]->back);			
		} 
		else 
			/*
			Se verifica daca s-a efectuat o rotatie completa. 
			In caz afirmativ, se anuleaza rotatia pe X, se rearanjeaza cuburile si se inverseaza fetele acestora.
			*/
			if ((angle > 0.0f) && (spinX[side] == 90.0f))
			{
				spinX[side] = 0;
	    
				Cube *aux = cube[side][0][2];
				cube[side][0][2] = cube[side][2][2];
				cube[side][2][2] = cube[side][2][0];
				cube[side][2][0] = cube[side][0][0];
				cube[side][0][0] = aux;
	    
				aux = cube[side][0][1];
				cube[side][0][1] = cube[side][1][2];
				cube[side][1][2] = cube[side][2][1];
				cube[side][2][1] = cube[side][1][0];
				cube[side][1][0] = aux;

				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3 ; j++)
						cube[side][i][j]->switchFaces(&cube[side][i][j]->back,&cube[side][i][j]->bottom,&cube[side][i][j]->front,&cube[side][i][j]->top);
			}
	
	}
	/*
	Daca toate blocurile au fost rotite, atunci se anuleaza rotatiile pe X si se aduna la rotatia globala pe X.
	*/
	  if ((spinX[0] == spinX[1] ||
       spinX[0] == spinX[1]-90 ||
       spinX[0] == spinX[1]+90) &&
      (spinX[1] == spinX[2] ||
       spinX[1] == spinX[2]-90 ||
       spinX[1] == spinX[2]+90)) {

		rotationX += spinX[0];
		spinX[0] = spinX[1] = spinX[2] = 0;
	  }
	
}


/*
Rotatie pe Y.
@param Blocul ce urmeaza a fi rotit.
@param Unghiul de rotatie.
*/
void Rubik::rotateY(int side, float angle){

	/*
	Se verifica daca rotatia pe X si Z este nula.
	Doar in acest caz se poate efectua rotatia pe Y a blocului dat.
	*/

	if ((fmod((double)spinX[0],90.0) == fmod((double)spinX[1],90.0)) 
		&& (fmod((double)spinX[1],90.0) == fmod((double)spinX[2],90.0)) 
		&& (fmod((double)spinX[2],90.0) == 0.0 )
		&& (fmod((double)spinZ[0],90.0) == fmod((double)spinZ[1],90.0)) 
		&& (fmod((double)spinZ[1],90.0) == fmod((double)spinZ[2],90.0)) 
		&& (fmod((double)spinZ[2],90.0) == 0.0 ))
	{
		spinY[side] += angle;

		/*
		Se verifica daca s-a efectuat o rotatie completa. 
		In caz afirmativ, se anuleaza rotatia pe Y se rearanjeaza cuburile si se inverseaza fetele acestora.
		*/

		if ((angle < 0.0f) && (spinY[side] == -90.0f))
		{
			spinY[side] = 0;
    
			Cube *aux = cube[0][side][2];
			cube[0][side][2] = cube[2][side][2];
			cube[2][side][2] = cube[2][side][0];
			cube[2][side][0] = cube[0][side][0];
			cube[0][side][0] = aux;
    
			aux = cube[0][side][1];
			cube[0][side][1] = cube[1][side][2];
			cube[1][side][2] = cube[2][side][1];
			cube[2][side][1] = cube[1][side][0];
			cube[1][side][0] = aux;

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3 ; j++)
					cube[i][side][j]->switchFaces(&cube[i][side][j]->back,&cube[i][side][j]->left,&cube[i][side][j]->front,&cube[i][side][j]->right);
		
		} 
		else 

			/*
			Se verifica daca s-a efectuat o rotatie completa. 
			In caz afirmativ, se anuleaza rotatia pe Y, se rearanjeaza cuburile si se inverseaza fetele acestora.
			*/

			if ((angle > 0.0f) && (spinY[side] == 90.0f))
			{
				spinY[side] = 0;
	    
				Cube *aux = cube[0][side][0];
				cube[0][side][0] = cube[2][side][0];
				cube[2][side][0] = cube[2][side][2];
				cube[2][side][2] = cube[0][side][2];
				cube[0][side][2] = aux;
	    
				aux = cube[1][side][0];
				cube[1][side][0] = cube[2][side][1];
				cube[2][side][1] = cube[1][side][2];
				cube[1][side][2] = cube[0][side][1];
				cube[0][side][1] = aux;

				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3 ; j++)
						cube[i][side][j]->switchFaces(&cube[i][side][j]->right,&cube[i][side][j]->front,&cube[i][side][j]->left,&cube[i][side][j]->back);
			}
	}

	/*
	Daca toate blocurile au fost rotite, atunci se anuleaza rotatiile pe Y si se aduna la rotatia globala pe Y.
	*/
	  if ((spinY[0] == spinY[1] ||
       spinY[0] == spinY[1]-90 ||
       spinY[0] == spinY[1]+90) &&
      (spinY[1] == spinY[2] ||
       spinY[1] == spinY[2]-90 ||
       spinY[1] == spinY[2]+90)) {
    
		rotationY += spinY[0];
		spinY[0] = spinY[1] = spinY[2] = 0;
  }
}

/*
Rotatie pe Y.
@param Blocul ce urmeaza a fi rotit.
@param Unghiul de rotatie.
*/

void Rubik::rotateZ(int side, float angle){

	/*
	Se verifica daca rotatia pe X si Y este nula.
	Doar in acest caz se poate efectua rotatia pe Z a blocului dat.
	*/

	if ((fmod((double)spinY[0],90.0) == fmod((double)spinY[1],90.0)) 
		&& (fmod((double)spinY[1],90.0) == fmod((double)spinY[2],90.0)) 
		&& (fmod((double)spinY[2],90.0) == 0.0 )
		&& (fmod((double)spinX[0],90.0) == fmod((double)spinX[1],90.0)) 
		&& (fmod((double)spinX[1],90.0) == fmod((double)spinX[2],90.0)) 
		&& (fmod((double)spinX[2],90.0) == 0.0 ))
	{
		spinZ[side] += angle;

		/*
		Se verifica daca s-a efectuat o rotatie completa. 
		In caz afirmativ, se anuleaza rotatia pe Z, se rearanjeaza cuburile si se inverseaza fetele acestora.
		*/

		if ((angle < 0.0f) && (spinZ[side] == -90.0f))
		{
			spinZ[side] = 0;
    
			Cube *aux = cube[0][0][side];
			cube[0][0][side] = cube[2][0][side];
			cube[2][0][side] = cube[2][2][side];
			cube[2][2][side] = cube[0][2][side];
			cube[0][2][side] = aux;
    
			aux = cube[1][0][side];
			cube[1][0][side] = cube[2][1][side];
			cube[2][1][side] = cube[1][2][side];
			cube[1][2][side] = cube[0][1][side];
			cube[0][1][side] = aux;

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3 ; j++)
					cube[i][j][side]->switchFaces(&cube[i][j][side]->top,&cube[i][j][side]->left,&cube[i][j][side]->bottom,&cube[i][j][side]->right);
			
		} 
		else 

			/*
			Se verifica daca s-a efectuat o rotatie completa. 
			In caz afirmativ, se anuleaza rotatia pe Z, se rearanjeaza cuburile si se inverseaza fetele acestora.
			*/

			if ((angle > 0.0f) && (spinZ[side] == 90.0f))
			{
				spinZ[side] = 0;
	    
			Cube *aux = cube[0][2][side];
			cube[0][2][side] = cube[2][2][side];
			cube[2][2][side] = cube[2][0][side];
			cube[2][0][side] = cube[0][0][side];
			cube[0][0][side] = aux;
    
			aux = cube[0][1][side];
			cube[0][1][side] = cube[1][2][side];
			cube[1][2][side] = cube[2][1][side];
			cube[2][1][side] = cube[1][0][side];
			cube[1][0][side] = aux;

				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3 ; j++)
						cube[i][j][side]->switchFaces(&cube[i][j][side]->right,&cube[i][j][side]->bottom,&cube[i][j][side]->left,&cube[i][j][side]->top);
			}
	}

	/*
	Daca toate blocurile au fost rotite, atunci se anuleaza rotatiile pe Z si se aduna la rotatia globala pe Z.
	*/
	  if ((spinZ[0] == spinZ[1] ||
       spinZ[0] == spinZ[1]-90 ||
       spinZ[0] == spinZ[1]+90) &&
      (spinZ[1] == spinZ[2] ||
       spinZ[1] == spinZ[2]-90 ||
       spinZ[1] == spinZ[2]+90)) {
   
		rotationZ += spinZ[0];
		spinZ[0] = spinZ[1] = spinZ[2] = 0;
  }
}
