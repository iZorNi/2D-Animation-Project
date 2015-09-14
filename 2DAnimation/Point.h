#pragma once
class Point
{

private:
	int x, y;

public:
	Point();
	Point(int posX, int posY);
	Point(const Point& A);
	int getX();
	int getY();
	int setX(int posX);
	int setY(int posY);
};

