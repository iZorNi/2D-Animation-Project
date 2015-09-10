#pragma once
#include "stdafx.h"



class Frame
{
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

	std::unique_ptr<Frame::Diff> getDiff(std::weak_ptr<Frame> const frame);

	std::pair<long, std::shared_ptr<Point>> getPoint(long id);

	std::pair<long, std::shared_ptr<Point>> getNextPoint();

	std::pair<long, std::shared_ptr<Point>> resetCurrentPoint();

	std::pair<long, long> resetCurrentEdge();

	std::pair<long, long> getNextEdge();

	class PointIterator
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
		PointIterator(Frame* frame, MIter begin, MIter end, std::function<bool(dPoint&)> qualifier = &allPointsQualifier);
		PointIterator(Frame* frame, MIter begin, MIter end, long a, std::function<bool(dPoint&)> qualifier = &pointWithIdQualifier);
		PointIterator(Frame* frame, MIter begin, MIter end, int x, int y, std::function<bool(dPoint&)> qualifier = &pointWithCoordQualifier);
		PointIterator(const PointIterator& value);
		PointIterator& setQualifierCoords(int x, int y);
		PointIterator& setIdQualifier(long id);
		bool pointWithCoordQualifier(dPoint& point);
		bool pointWithIdQualifier(dPoint& point);
		bool allPointsQualifier(dPoint& point);
		Point operator*();
		std::shared_ptr<Point> operator->();
		PointIterator& operator++();
		PointIterator operator++(int);
		PointIterator& operator--();
		PointIterator operator--(int);
		PointIterator& operator=(const PointIterator& value);
		bool operator==(PointIterator& value);
		PointIterator end();
		PointIterator begin();
		bool isPicked();
	};

	class EdgeIterator
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
		EdgeIterator(Frame* frame, SIter begin, SIter end, long a,long b, std::function<bool(dEdge&)> qualifier = &edgeWithPointsQualifier);
		EdgeIterator(Frame* frame, SIter begin, SIter end, int x, int y, std::function<bool(dEdge&)> qualifier = &edgeByCoordQualifier);
		EdgeIterator& setQualifierCoords(int x, int y);
		EdgeIterator& setQualifierPoints(long a, long b);
		bool edgeWithPointsQualifier(dEdge& edge);
		bool edgeByCoordQualifier(dEdge& edge);
		bool allEdgesQualifier(dEdge& edge);
		std::pair<long,long> operator*();
		Frame::SIter operator->();
		EdgeIterator& operator++();
		EdgeIterator operator++(int);
		EdgeIterator& operator--();
		EdgeIterator operator--(int);
		EdgeIterator& operator=(const EdgeIterator& value);
		bool operator==(EdgeIterator& value);
		EdgeIterator end();
		EdgeIterator begin();
		std::pair<std::pair<int, int>, std::pair<int, int>> getPointsCoord();
	};

};


