#pragma once
#include "stdafx.h"
#include "PointsContainer.h"
#include "EdgesContainer.h"


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
	unsigned int id;
	std::shared_ptr<PointsContainer> points;
	std::shared_ptr<EdgesContainer> edges;

	PointsContainer::PointIterator pickedPoint;
	EdgesContainer::EdgeIterator pickedEdge;

	void removeEdgesConnectedToPoint(long int id);

	bool pickEdge(int x, int y);
	void getDifferenceBetweenPoints(std::shared_ptr<Frame> const inputFrame, std::unique_ptr<Diff>& result);
	void getDifferenceBetweenEdges(std::shared_ptr<Frame> const inputFrame, std::unique_ptr<Diff>& result);

public:

	Frame(unsigned int id=0);
	Frame(const Frame& frame);
	//returns point id

	unsigned int getID();

	void setID(unsigned int id);

	int getNumberOfPoints();

	int getNumberOfEdges();

	~Frame();

	std::unique_ptr<Frame::Diff> getDifference(std::weak_ptr<Frame> const frame);

	void freePickedPoint();

	void movePoint(int x, int y);

	PointsContainer::PointIterator addPoint(int x, int y);
	PointsContainer::PointIterator addPoint(long int id, int x, int y);
	
	EdgesContainer::iterator addEdge(long int A, long int B);

	bool pickPoint(int x, int y);
	bool pickPoint(long int id);

	void removePoint(int x, int y);
	void removePoint(long int id);
	void removePoint(PointsContainer::iterator pointIter);

	void removeEdge(EdgesContainer::iterator edgeIter);
	void removeEdgeByCoord(int x, int y);
	void removeEdgeByPoints(long int a, long int b);

	bool isPickedPoint(long int id);
	bool isPickedPoint(PointsContainer::iterator pointIter);

	PointsContainer::iterator getPoint(long id);

	PointsContainer::iterator pointsEnd();
	PointsContainer::iterator pointsBegin();
	EdgesContainer::iterator edgesEnd();
	EdgesContainer::iterator edgesBegin();

};
