#include "Ball.h"

Ball::Ball(std::vector<Object2d> object)
{
	this->object = object;
}

Ball::~Ball(void)
{
}

Ball::setContact(Point2d point){
	this->contact = point;
}
