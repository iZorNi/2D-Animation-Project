#pragma once
#include "Point.h"
#include <set>
#include <map>
#include <list>
#include <memory>

using std::set;
using std::map;
using std::list;


class Frame
{
	uint id;
	long int point_next_id;
	std::map<long int,std::shared_ptr<Point>> points;
	std::set<std::pair<long int,long int>> edges;
	std::set < std::pair<long, std::shared_ptr<Point>>> sPoints;
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

	bool pickPoint(long int id);

	bool tryPickEdge(int x, int y);

	bool pickEdge(long int a, long int b);

	long int addPoint(uint x, uint y);

	long int addPoint(long int id, uint x, uint y);

	void movePoint(int x, int y);

	void removePoint();

	bool addEdge(long int A, long int B);

	void removeEdge();

	void freeEdge();

	void freePoint();

	int getNumberOfPoints();

	int getNumberOfEdges();

	int getPickedPointId();

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

};


