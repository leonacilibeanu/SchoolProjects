#include "Camera.h"

Camera::Camera(){
}
Camera::~Camera(){
}

void Camera::init(Vector3D position, Vector3D up, Vector3D right, Vector3D ref){
	
	time = 0.0f;

	this->center  = ref;
	this->position = position;
	this->right = right;
	this->up = up;
	this->forward = this->center - this->position;

}

void Camera::translate_Forward(float dist){

	position.x += forward.x * dist;
	position.z += forward.z * dist;
	center = position + forward;

}

void Camera::translate_Right(float dist){

	position = position + right * dist;
	center = position + forward;

}

void Camera::rotateTPS(float longitude, float latitude){

	time = 1.0f;

	float latitudeTime = latitude*time;
    float longitudeTime = longitude*time;

    Quaternion rotation(longitudeTime, latitudeTime, 0.0f);
    Quaternion offsetV(0.0f, -forward.x, -forward.y, -forward.z);
    Quaternion p = rotation.conjugate() * offsetV * rotation;

    Vector3D newOffsetV(p.x, p.y, p.z);
    Vector3D newPosition = newOffsetV + center;

	position = newPosition;
	forward = center - position;
}

void Camera::rotateFPS_OY(float angle){

	forward = forward * cos(angle) + right * sin(angle);
	right = forward.CrossProduct(up);

	center = forward + position;
}

void Camera::render(){

	gluLookAt(  position.x, position.y, position.z, 
				center.x, center.y, center.z,
				up.x, up.y, up.z);
}