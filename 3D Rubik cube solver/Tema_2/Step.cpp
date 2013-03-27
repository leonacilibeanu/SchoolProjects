#include "Step.h"

Step::Step(void)
{
}

Step::~Step(void)
{
}

Step::Step(unsigned char axis, int side, float angle){
	this->axis = axis;
	this->angle = angle;
	this->side = side;
}
