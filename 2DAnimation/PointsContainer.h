#pragma once
#include "stdafx.h"

class PointsContainer
{
public:
	class PointIterator;
	typedef std::map<long int, std::shared_ptr<Point>>::iterator PointsMapIterator;
	typedef std::map<long int, std::shared_ptr<Point>>::const_iterator PointsMapIterator_const;
	typedef PointIterator iterator;
private:
	std::map<long int, std::shared_ptr<Point>> points;
	long int pointNextId;
	PointsMapIterator findPoint(int x, int y);
	void removePoint(PointsMapIterator it);
	iterator createIterator(PointsMapIterator it);
public:

	PointsContainer();
	~PointsContainer();
	//as usual
	iterator begin();
	iterator end();
	//returns itearator pointing to added point
	iterator addPoint(int x, int y);
	iterator addPoint(long id, int x, int y);
	iterator copyPoint(iterator pointIterator);
	void removePoint(iterator point);
	void removePoint(int x, int y);
	void removePoint(long int id);
	//returns iterator to being searched point or Points.end() if such doesn't exist
	iterator getPoint(int x, int y);
	iterator getPoint(long int id);
	int size();
};

class PointsContainer::PointIterator:public  Iterator<std::map<long int, std::shared_ptr<Point>>>
{
	friend class PointsContainer;
protected:
	PointIterator(PointsMapIterator begin, PointsMapIterator end);
	PointIterator(PointsMapIterator begin, PointsMapIterator end, PointsMapIterator current);
public:
	PointIterator();
	virtual ~PointIterator();
	PointIterator(const PointIterator& value);
	virtual PointIterator& operator=(const PointIterator& value);
	Point operator*();
	std::shared_ptr<Point> operator->();
	long getId();
};
