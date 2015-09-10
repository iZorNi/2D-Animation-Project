#pragma once
#include "stdafx.h"



class Frame
{
public: 
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
private:
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
	void getDifferenceBetweenPoints(std::shared_ptr<Frame> const inputFrame, std::unique_ptr<Diff>& result);
	void getDifferenceBetweenEdges(std::shared_ptr<Frame> const inputFrame, std::unique_ptr<Diff>& result);

public:
	class PointIterator;
	class EdgeIterator;

	const std::pair<long int, std::shared_ptr<Point>> failurePoint = std::make_pair(-1, nullptr);

	Frame();
	Frame(uint id);
	Frame(const Frame& frame);
	//returns point id

	uint getID();

	void setID(uint id);

	int getNumberOfPoints();

	int getNumberOfEdges();

	~Frame();



	std::unique_ptr<Frame::Diff> getDifference(std::weak_ptr<Frame> const frame);

	void freePickedPoint()
	{
		pickedPoint = points.end();
	}

	long int addPoint(uint x, uint y);

	long int addPoint(long int id, uint x, uint y);

	bool addEdge(long int A, long int B);

	void movePoint(int x, int y);

	//iterates all points
	PointIterator getPointIterator(std::function<bool(std::pair<const long, std::shared_ptr<Point>>&)> qualifier = nullptr);
	//iterates all points with id == id
	PointIterator getPointIterator(long int id, std::function<bool(std::pair<const long, std::shared_ptr<Point>>&)> qualifier = nullptr);
	//itearates all points with (x,y) coordinates
	PointIterator getPointIterator(int x, int y, std::function<bool(std::pair<const long, std::shared_ptr<Point>>&)> qualifier = nullptr);

	//iterates all edges
	EdgeIterator getEdgeIterator(std::function<bool(std::pair<long, long>&)> qualifier = nullptr);
	//iterates all edges with points a and b as vertices
	EdgeIterator getEdgeIterator(long int a, long int b, std::function<bool(std::pair<long, long>&)> qualifier = nullptr);
	//iterates all edges which lie on point(x,y)
	EdgeIterator getEdgeIterator(int x, int y, std::function<bool(std::pair<long, long>&)> qualifier = nullptr);

	void removeEdge(EdgeIterator edgeIter);
	void removePoint(PointIterator pointIter);
	bool tryPickPoint(PointIterator pointIter);

	//rewrite using iterator
	bool tryPickPoint(int x, int y);

	bool tryPickPoint(long int id);

	void removePoint(int x, int y);

	void removeEdge(int x, int y);

	void removeEdgeByPoints(long int a, long int b);

	//no need because of PointIterator
	bool isPickedPoint(long int id);

	// will be implemented with PointIterator and EdgeIterator
	std::pair<long, std::shared_ptr<Point>> getPoint(long id);

	std::pair<long, std::shared_ptr<Point>> getNextPoint();

	std::pair<long, std::shared_ptr<Point>> getFirstPoint();

	std::pair<long, long> getFirstEdge();

	std::pair<long, long> getNextEdge();

	

};

class Frame::PointIterator
{
	typedef std::pair<const long, std::shared_ptr<Point>> dPoint;
	MIter _begin;
	MIter _end;
	MIter _current;
	Frame* frame;
	std::function<bool(dPoint&)> qualifier;
	void initCurrent();
	long qualifierID;
	long qualifierCoordinateX, qualifierCoordinateY;
public:
	//static bool pointWithCoordQualifier1(dPoint& point);
	bool pointWithCoordQualifier(dPoint& point);
	bool pointWithIdQualifier(dPoint& point);
	bool allPointsQualifier(dPoint& point);
	PointIterator(Frame* frame, MIter begin, MIter end, std::function<bool(dPoint&)> qualifier = nullptr);
	PointIterator(Frame* frame, MIter begin, MIter end, long a, std::function<bool(dPoint&)> qualifier = nullptr);
	PointIterator(Frame* frame, MIter begin, MIter end, int x, int y, std::function<bool(dPoint&)> qualifier = nullptr);
	PointIterator(const PointIterator& value);
	PointIterator& setQualifierCoords(int x, int y);
	PointIterator& setIdQualifier(long id);
	Point operator*();
	std::shared_ptr<Point> operator->();
	PointIterator& operator++();
	PointIterator operator++(int);
	PointIterator& operator--();
	PointIterator operator--(int);
	PointIterator& operator=(const PointIterator& value);
	bool operator==(PointIterator& value);
	bool operator!=(PointIterator& value);
	PointIterator end();
	PointIterator begin();
	bool isPicked();
	long getId();
};

class Frame::EdgeIterator
{
	typedef const std::pair<long, long> dEdge;
	SIter _begin;
	SIter _end;
	SIter _current;
	std::function<bool(dEdge&)> qualifier;
	Frame* frame;
	long qualifierPointIdA, qualifierPointIdB;
	long qualifierCoordinateX, qualifierCoordinateY;
	void initCurrent();
public:
	EdgeIterator(Frame* frame, SIter begin, SIter end, std::function<bool(dEdge&)> qualifier = &allEdgesQualifier);
	EdgeIterator(Frame* frame, SIter begin, SIter end, long a, long b, std::function<bool(dEdge&)> qualifier = &edgeWithPointsQualifier);
	EdgeIterator(Frame* frame, SIter begin, SIter end, int x, int y, std::function<bool(dEdge&)> qualifier = &edgeByCoordQualifier);
	EdgeIterator& setQualifierCoords(int x, int y);
	EdgeIterator& setQualifierPoints(long a, long b);
	bool edgeWithPointsQualifier(dEdge& edge);
	bool edgeByCoordQualifier(dEdge& edge);
	bool allEdgesQualifier(dEdge& edge);
	std::pair<long, long> operator*();
	Frame::SIter operator->();
	EdgeIterator& operator++();
	EdgeIterator operator++(int);
	EdgeIterator& operator--();
	EdgeIterator operator--(int);
	EdgeIterator& operator=(const EdgeIterator& value);
	bool operator==(EdgeIterator& value);
	bool operator!=(EdgeIterator& value);
	EdgeIterator end();
	EdgeIterator begin();
	std::pair<std::pair<int, int>, std::pair<int, int>> getPointsCoord();
};
