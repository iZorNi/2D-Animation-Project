#include "Point.h"

typedef unsigned int uint;


Point::Point()
{
	x = 0;
	y = 0;
}

uint Point::getX()
{
	return x;
}

uint Point::getY()
{
	return y;
}

uint Point::setX(uint posX)
{
	return this->x = posX;
}

uint Point::setY(uint posY)
{
	return this->y = posY;
}

/*long int Point::getId()
{
	return this->id;
}

long int Point::setId(long int id)
{
	this->id = id;
	return this->id;
}

long int Point::decreaseId()
{
	return --id;
}*/
