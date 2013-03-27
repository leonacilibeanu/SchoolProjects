#pragma once

class Ball
{
public:
	Ball(void);
	~Ball(void);

	void setContact(Point2d point);

public:
	Object2d object; //obiectul minge
	Point2d contact;
};
