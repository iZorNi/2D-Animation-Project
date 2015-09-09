#pragma once
#include "stdafx.h"



class Frame
{

	typedef std::pair<long, std::shared_ptr<Point>> dPoint;
	uint id;
	long int point_next_id;
	std::map<long int,std::shared_ptr<Point>> points;
	std::set<std::pair<long int,long int>> edges;
	
	typedef std::set<std::pair<long int, long int>>::iterator SIter;
	typedef std::set<std::pair<long int, long int>>::const_iterator SIter_const;
	typedef std::map<long int, std::shared_ptr<Point>>::iterator MIter;
	typedef std::map<long int, std::shared_ptr<Point>>::const_iterator MIter_const;

	MIter pickedPoint, currentPoint;
	SIter pickedEdge, currentEdge;

	MIter getPointByCoord(int x, int y);
	SIter getEdgeByCoord(int x, int y);

	//returns iterator to the edge pair
	void removeConnectedEdges(long int id);

	bool tryPickEdge(int x, int y);

public:
	const std::pair<long int, std::shared_ptr<Point>> failurePoint = std::make_pair(-1, nullptr);

	Frame();
	Frame(uint id);
	Frame(const Frame& frame);
	//returns point id

	uint getID();

	void setID(uint id);

	~Frame();
	bool tryPickPoint(int x, int y);

	bool tryPickPoint(long int id);
	
	void freePickedPoint()
	{
		pickedPoint = points.end();
	}

	long int addPoint(uint x, uint y);

	long int addPoint(long int id, uint x, uint y);

	void movePoint(int x, int y);

	void removePoint(int x, int y);

	bool addEdge(long int A, long int B);

	void removeEdge(int x, int y);

	void removeEdgeByPoints(long int a, long int b);

	int getNumberOfPoints();

	int getNumberOfEdges();

	bool isPickedPoint(long int id);

	struct Diff
	{
		enum status
		{
			ADDED = 0, MOVED, REMOVED
		};
		unsigned int id;
		std::map<long int, std::pair<std::shared_ptr<Point>, status>> points;
		std::set<std::pair< std::pair<long int, long int>, status>> edges;
	};

	std::unique_ptr<Frame::Diff> getDiff(const Frame & frame);

	std::pair<long, std::shared_ptr<Point>> getPoint(long id);

	std::pair<long, std::shared_ptr<Point>> getNextPoint();

	std::pair<long, std::shared_ptr<Point>> resetCurrentPoint();

	std::pair<long, long> resetCurrentEdge();

	std::pair<long, long> getNextEdge();

	//class PointIterator
	//{
	//	std::map<long int, std::shared_ptr<Point>> begin;
	//	std::map<long int, std::shared_ptr<Point>> end;
	//	std::function<bool(long)> filterPoints;
	//	long searchId;
	//	long searchCoordinateX, searchCoordinateY;
	//public:
	//	PointIterator(std::function<bool(long, long)> criteria = [](dPoint point){return true});
	//	PointIterator(long id, std::function<bool(long,long)> criteria = [this](dPoint point) {return (point.first == this->searchId);});
	//	PointIterator(int x, int y, std::function<bool(int,int,int,int)> criteria = [&](dPoint point) {return (point.second->getX() == searchCoordinateX && point.second->getX() == searchCoordinateY();});

	//};

};


