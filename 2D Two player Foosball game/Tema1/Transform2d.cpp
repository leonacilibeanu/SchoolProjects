#pragma once
#include "Transform2d.h"
#include "Support2d.h"

void Transform2d::translate(Point2d *pct, float tx, float ty){
	pct->x += tx;
	pct->y += ty;
}

void Transform2d::rotate(Point2d *pct, float angleInRadians){
	double xx = pct->x * cos(angleInRadians) - pct->y * sin(angleInRadians);
	double yy = pct->x * sin(angleInRadians) + pct->y * cos(angleInRadians);
	pct->x = static_cast<float>(xx);
	pct->y = static_cast<float>(yy);
}

void Transform2d::rotateRelativeToAnotherPoint(Point2d *pct, Point2d *ref, float angleInRadians){
	pct->translate(-ref->x, -ref->y);
	pct->rotateRelativeToOrigin(angleInRadians);
	pct->translate(ref->x, ref->y);
}
void Transform2d::scale(Point2d *pct, float sx, float sy){
	pct->x *= sx;
	pct->y *= sy;
}
void Transform2d::scaleRelativeToAnotherPoint(Point2d *pct, Point2d *ref, float sx, float sy){
	pct->translate(-ref->x, -ref->y);
	pct->scale(sx, sy);
	pct->translate(ref->x, ref->y);
}
