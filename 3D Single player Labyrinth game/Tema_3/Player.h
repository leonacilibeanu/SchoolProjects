#pragma once
#include "glut\glut.h"
#include "MathLibrary.h"
#include "Camera.h"
#include "Image.h"

class Player
{
public:
	Player();
	Player(char* filename, int texWidth, int texHeight);
	~Player(void);

	void init(Vector3D position, Vector3D forward, Vector3D up, Vector3D right, float radius);

	void limitToYAxis(bool limit);

    void direct(float heading, float pitch, float roll);
	void rotateFPS_OY(float angle);
	void translate_Forward(float dist, Vector3D f);
	void translate_Right(float dist);
	void updateAxes();
   
    void setTotalMatrix(Matrix4 &totalMatrix);
    
    void update();

	Quaternion eulerToQuaternion(Matrix4 &m, float heading, float pitch, float roll);

    void getAxes();

	void texture(char* file);
	void drawPlayer();
	void drawOpponent();

public:

	float time;
	float type;

	Vector3D center;
    Matrix4 totalMatrix;
    Quaternion orientation;
    Quaternion rotation;
    
    Vector3D right;
    Vector3D up;
    Vector3D forward;
    
    Vector3D position;
    Vector3D speed;
    Vector3D eOrient;
    Vector3D eRotate;
    
    bool limitY;
	 
	float radius;
	Camera playerCam;
	GLuint face;
};
