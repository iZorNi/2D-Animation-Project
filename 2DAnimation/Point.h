#pragma once
typedef unsigned int uint;
class Point
{

private:
	uint x, y;

public:
	Point();
	Point(uint posX, uint posY) :x(posX), y(posY) { };
	Point(const Point& A)
	{
		this->x = A.x;
		this->y = A.y;
	}
	uint getX();
	uint getY();
	uint setX(uint posX);
	uint setY(uint posY);
};

