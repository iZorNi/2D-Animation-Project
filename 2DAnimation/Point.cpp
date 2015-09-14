#include "Point.h"


Point::Point()
{
	x = 0;
	y = 0;
}

Point::Point(int posX, int posY) 
	:x(posX), y(posY) 
{ };

Point::Point(const Point& A)
	: x(A.x) , y(A.y)
{}

int Point::getX()
{
	return x;
}

int Point::getY()
{
	return y;
}

int Point::setX(int posX)
{
	return this->x = posX;
}

int Point::setY(int posY)
{
	return this->y = posY;
}
