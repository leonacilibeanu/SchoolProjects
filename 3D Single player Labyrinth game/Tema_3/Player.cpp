#include "Player.h"

Player::Player()
{
}
Player::Player(char* filename, int texWidth,int texHeight)
{
	Image *image = new Image(filename,texWidth,texHeight);
	face = image->textureID();
}

Player::~Player(void)
{
}

void Player::init(Vector3D position, Vector3D forward, Vector3D up, Vector3D right, float radius){
	time = 0.0f;
    
    speed  = Vector3D(0.0f, 0.0f, 0.0f);
    eOrient = Vector3D(0.0f, 0.0f, 0.0f);
    eRotate = Vector3D(0.0f, 0.0f, 0.0f);

	this->radius = radius;
	limitY = false;

	totalMatrix.identity();
    orientation.identity();
    rotation.identity();

	this->position = position;
	this->forward = forward;
	this->up = up;
	this->right = right;
	center = forward + position;

	totalMatrix[3][0] = position.x;
    totalMatrix[3][1] = position.y;
    totalMatrix[3][2] = position.z;
}

void Player::limitToYAxis(bool limit)
{
    limitY = limit;
}

void Player::direct(float heading, float pitch, float roll)
{
    eOrient.x += pitch;
    eOrient.y += heading;
    eOrient.z += roll;

    if (eOrient.x > 360.0f)
        eOrient.x -= 360.0f;

    if (eOrient.x < -360.0f)
        eOrient.x += 360.0f;

    if (eOrient.y > 360.0f)
        eOrient.y -= 360.0f;

    if (eOrient.y < -360.0f)
        eOrient.y += 360.0f;

    if (eOrient.z > 360.0f)
        eOrient.z -= 360.0f;

    if (eOrient.z < -360.0f)
        eOrient.z += 360.0f;
}

void Player::setTotalMatrix(Matrix4 &totalMatrix)
{
    this->totalMatrix = totalMatrix;
    orientation.fromMatrix(totalMatrix);
    position = Vector3D(totalMatrix[3][0], totalMatrix[3][1], totalMatrix[3][2]);
    updateAxes();
}

void Player::update()
{
	time = 1.0f;

    Vector3D speedFrame, eOrientFrame, eRotateFrame;
    Vector3D oldPos, h;
    Quaternion temp;

    speedFrame = speed * time;
    eOrientFrame = eOrient * time;
    eRotateFrame = eRotate * time;
    
    updateAxes();

    oldPos = position;

    position += right * speedFrame.x;
    position += up * speedFrame.y;
    position += forward * speedFrame.z;

    h = position - oldPos;
    h.Normalize();
    
    temp = eulerToQuaternion(orientation.toMatrix4(), eOrientFrame.y, eOrientFrame.x, eOrientFrame.z);
    
	if (h.Dot(forward) < 0.0f)
        temp = temp.inverse();

    orientation *= temp;
    orientation.normalize();

    temp = eulerToQuaternion(rotation.toMatrix4(), eRotateFrame.y,
            eRotateFrame.x, eRotateFrame.z);
    
    rotation *= temp;
    rotation.normalize();

    temp = rotation * orientation;
    temp.normalize();

    totalMatrix = temp.toMatrix4();
    totalMatrix[3][0] = position.x;
    totalMatrix[3][1] = position.y;
    totalMatrix[3][2] = position.z;

    speed = Vector3D(0.0f, 0.0f, 0.0f);
    eOrient = Vector3D(0.0f, 0.0f, 0.0f);
    eRotate = Vector3D(0.0f, 0.0f, 0.0f);
}

Quaternion Player::eulerToQuaternion(Matrix4 &m, float heading, float pitch, float roll)
{
    Quaternion result = Quaternion::IDENTITY;
    Quaternion rotation = Quaternion::IDENTITY;
    Vector3D tempXAxis = Vector3D(m[0][0], m[0][1], m[0][2]);
    Vector3D tempYAxis = Vector3D(m[1][0], m[1][1], m[1][2]);
    Vector3D tempZAxis = Vector3D(m[2][0], m[2][1], m[2][2]);   

    if (heading != 0.0f)
    {
        if (limitY)
            rotation.fromAxisAngle(Vector3D(0.0f, 1.0f, 0.0f), heading);
        else
            rotation.fromAxisAngle(tempYAxis, heading);

        result *= rotation;
    }

    if (pitch != 0.0f)
    {
        rotation.fromAxisAngle(tempXAxis, pitch);
        result *= rotation;
    }

    if (roll != 0.0f)
    {
        rotation.fromAxisAngle(tempZAxis, roll);
        result *= rotation;
    }

    return result;
}

void Player::updateAxes()
{
    Matrix4 m = orientation.toMatrix4();

    right = Vector3D(m[0][0], m[0][1], m[0][2]);
    right.Normalize();
    
    up = Vector3D(m[1][0], m[1][1], m[1][2]);
    up.Normalize();
    
    forward = Vector3D(-m[2][0], -m[2][1], -m[2][2]);
    forward.Normalize();
}

void Player::translate_Forward(float dist, Vector3D f){

	position.x += f.x * dist;
	position.z += f.z * dist;
	center = position + f;

	totalMatrix[3][0] += f.x * dist;
    totalMatrix[3][2] += f.z * dist;

}


void Player::rotateFPS_OY(float angle){

	forward = forward * cosf(angle) + right * sinf( angle );
	right = forward.CrossProduct(up);

	center = forward + position;

	totalMatrix[0][0] = cosf(angle) * totalMatrix[0][0] + sinf(angle) * totalMatrix[2][0];
	totalMatrix[0][1] = cosf(angle) * totalMatrix[0][1] + sinf(angle) * totalMatrix[2][1];
	totalMatrix[0][2] = cosf(angle) * totalMatrix[0][2] + sinf(angle) * totalMatrix[2][2];
	totalMatrix[0][3] = cosf(angle) * totalMatrix[0][3] + sinf(angle) * totalMatrix[2][3];

	totalMatrix[2][0] = -sinf(angle) * totalMatrix[0][0] + cosf(angle) * totalMatrix[2][0];
	totalMatrix[2][1] = -sinf(angle) * totalMatrix[0][1] + cosf(angle) * totalMatrix[2][1];
	totalMatrix[2][2] = -sinf(angle) * totalMatrix[0][2] + cosf(angle) * totalMatrix[2][2];
	totalMatrix[2][3] = -sinf(angle) * totalMatrix[0][3] + cosf(angle) * totalMatrix[2][3];

}

void Player::drawPlayer(){

	GLfloat material[] = {0.65f,0.65f,0.65f,0.65f,0.65f};

	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
	glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, material);

	glPushMatrix();

	glMultMatrixf(&totalMatrix[0][0]);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, face);

	GLUquadricObj* quadric = gluNewQuadric();

	gluQuadricDrawStyle ( quadric, GLU_FILL );
	gluQuadricNormals ( quadric, GLU_SMOOTH );
	gluQuadricTexture ( quadric, GL_TRUE );

	glRotatef(45, 1, 0, 0);

	gluSphere ( quadric, radius, 80, 80 );

	gluDeleteQuadric ( quadric );
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void Player::drawOpponent(){

	GLfloat material0[4] = {0.8f, 0.0f, 0.0f, 1.0f};
	GLfloat material1[4] = {0.0f, 0.8f, 0.0f, 1.0f};
	GLfloat material2[4] = {0.0f, 0.0f, 0.8f, 1.0f};
	GLfloat material3[4] = {0.8f, 0.8f, 0.0f, 1.0f};

	if (type == 0)
	{
		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material0);
		glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, material0);
	}

	if (type == 1)
	{
		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material1);
		glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, material1);
	}

	if (type == 2)
	{
		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material2);
		glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, material2);
	}

	if (type == 3)
	{
		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material3);
		glMaterialfv (GL_BACK, GL_AMBIENT_AND_DIFFUSE, material3);
	}

	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	glRotatef(90, 1, 0, 0);

	GLUquadricObj* quadric = gluNewQuadric();

	gluQuadricDrawStyle ( quadric, GLU_FILL );
	gluQuadricNormals ( quadric, GLU_SMOOTH );

	gluCylinder(quadric, radius, radius, 0.5f, 40, 40);

	gluDeleteQuadric ( quadric );
	glPopMatrix();

	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	glRotatef(90, 1, 0, 0);

	GLUquadricObj* quadric2 = gluNewQuadric();

	gluQuadricDrawStyle ( quadric2, GLU_FILL );
	gluQuadricNormals ( quadric2, GLU_SMOOTH );

	gluSphere(quadric2, radius, 40, 40);

	gluDeleteQuadric ( quadric2 );
	glPopMatrix();
}
