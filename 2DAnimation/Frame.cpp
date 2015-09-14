#include "Frame.h"

Frame::Frame(const Frame& frame):
	Frame(frame.id + 1)
{
	//exception not enough memory
	auto pointIterator = frame.points->begin();
	auto framePointsEnd = frame.points->end();
	while (pointIterator != framePointsEnd)
	{
		this->points->addPoint(pointIterator.getId(), pointIterator->getX(), pointIterator->getY());
		++pointIterator;
	}
	
	auto edgeIterator = frame.edges->begin();
	auto frameEdgesEnd = frame.edges->end();
	while (edgeIterator != frameEdgesEnd)
	{
		this->edges->addEdge(edgeIterator->first, edgeIterator->second);
		++edgeIterator;
	}
	
	this->pickedPoint = points->end();
	
	this->pickedEdge = this->edges->end();
}

Frame::Frame(unsigned int id)
	:id(id)
{
	points = std::make_shared<PointsContainer>();
	edges = std::make_shared<EdgesContainer>();
	pickedEdge = edges->end(); 
	pickedPoint = points->end();
}

Frame::~Frame()
{
	points.reset();
	edges.reset();
}

unsigned int Frame::getID()
{
	return this->id;
}

void Frame::setID(unsigned int id)
{
	this->id = id;
}

void Frame::freePickedPoint()
{
	pickedPoint = points->end();
}

PointsContainer::PointIterator Frame::addPoint(int x, int y)
{
	auto tmp = points->addPoint(x, y);
	freePickedPoint();
	return tmp;
}

PointsContainer::PointIterator Frame::addPoint(long int id, int x, int y)
{
	auto tmp = points->addPoint(id, x, y);
	freePickedPoint();
	return tmp;
}

EdgesContainer::iterator Frame::addEdge(long int A, long int B)
{
	return edges->addEdge(A, B);
}

void Frame::removePoint(int x, int y)
{
	points->removePoint(x, y);
	freePickedPoint();
}

void Frame::removePoint(long int id)
{
	points->removePoint(id);
	freePickedPoint();
}

void Frame::removePoint(PointsContainer::iterator pointIter)
{
	points->removePoint(pointIter);
	freePickedPoint();
}

void Frame::removeEdge(EdgesContainer::iterator edgeIter)
{
	edges->removeEdge(edgeIter);
}

void Frame::removeEdgeByCoord(int x, int y)
{
	freePickedPoint();
	edges->removeEdge(x, y, std::weak_ptr<PointsContainer>(points));
}

void Frame::removeEdgeByPoints(long int a, long int b)
{
	edges->removeEdge(a, b);
}

void Frame::removeEdgesConnectedToPoint(long int pointId)
{
	edges->removeEdgesWithPoint(pointId);
}

bool Frame::isPickedPoint(long int id)
{
	if (pickedPoint != points->end())
	{
		return pickedPoint.getId() == id;
	}
	else
	{
		return false;
	}
}

bool Frame::isPickedPoint(PointsContainer::iterator pointIter)
{
	return isPickedPoint(pointIter.getId());
}

PointsContainer::iterator Frame::getPoint(long id)
{
	return points->getPoint(id);
}

bool Frame::pickPoint(int x, int y)
{
	if (pickedPoint == points->end())
	{
		pickedPoint = points->getPoint(x, y);
	}
	else
	{
		auto tmp = points->getPoint(x, y);
		if (tmp != points->end())
		{
			if (tmp.getId() == pickedPoint.getId())
			{
				pickedPoint = points->end();
			}
			else
			{
				addEdge(pickedPoint.getId(), tmp.getId());
				freePickedPoint();
			}
		}
		else
		{
			freePickedPoint();
		}
	}
	return pickedPoint != points->end();
}

bool Frame::pickPoint(long int id)
{
	pickedPoint = points->getPoint(id);
	return pickedPoint != points->end();
}

void Frame::movePoint(int x, int y)
{
	if (pickedPoint != points->end())
	{
		pickedPoint->setX(x);
		pickedPoint->setY(y);
	}
}

bool Frame::pickEdge(int x, int y)
{
	pickedEdge = edges->getEdgeByCoord(x, y, std::weak_ptr<PointsContainer>(points));
	return (pickedEdge != edges->end());
}

int Frame::getNumberOfPoints()
{
	return points->size();
}

int Frame::getNumberOfEdges()
{
	return edges->size();
}

std::unique_ptr<Frame::Diff> Frame::getDifference(std::weak_ptr<Frame> const frame)
{
	if (frame.expired())
	{
		return nullptr;
	}
	std::shared_ptr<Frame> inputFrame = frame.lock();
	Diff* tmp = new Diff();
	std::unique_ptr<Diff> result = std::unique_ptr<Diff>(tmp);
	result->id = this->id;
	
	getDifferenceBetweenPoints(inputFrame, result);

	getDifferenceBetweenEdges(inputFrame, result);

	return result;
}

void addPointToDiff(PointsContainer::PointIterator iter, Frame::Diff::status status, std::unique_ptr<Frame::Diff>& result)
{
	long id = iter.getId();
	Point point = *iter;
	result->points.insert(std::make_pair(id, std::make_pair(std::make_shared<Point>(point), status)));
}

void Frame::getDifferenceBetweenPoints(std::shared_ptr<Frame> const inputFrame, std::unique_ptr<Frame::Diff>& result)
{
	auto thisFramePointIterator = this->points->begin();
	PointsContainer::iterator inputFramePointIterator;
	while (thisFramePointIterator != this->points->end())
	{
		inputFramePointIterator = inputFrame->points->getPoint(thisFramePointIterator.getId());
		if (inputFramePointIterator == inputFrame->points->end())
		{
			addPointToDiff(thisFramePointIterator, Diff::ADDED, result);
		}
		else
		{
			if (inputFramePointIterator->getX() != thisFramePointIterator->getX() || inputFramePointIterator->getY() != thisFramePointIterator->getY())
			{
				addPointToDiff(thisFramePointIterator, Diff::MOVED, result);
			}
		}
		++thisFramePointIterator;
	}

	inputFramePointIterator = inputFrame->points->begin();
	while (inputFramePointIterator != inputFrame->points->end())
	{
		thisFramePointIterator = this->points->getPoint(inputFramePointIterator.getId());
		if (thisFramePointIterator == this->points->end())
		{
			addPointToDiff(inputFramePointIterator, Diff::REMOVED, result);
		}
		++inputFramePointIterator;
	}
}

void Frame::getDifferenceBetweenEdges(std::shared_ptr<Frame> const inputFrame, std::unique_ptr<Frame::Diff>& result)
{
	auto thisFrameEdgeIterator = this->edges->begin();
	EdgesContainer::iterator inputFrameEdgeIterator;
	while (thisFrameEdgeIterator != this->edges->end())
	{
		inputFrameEdgeIterator = inputFrame->edges->getEdge(thisFrameEdgeIterator->first, thisFrameEdgeIterator->second);
		if (inputFrameEdgeIterator == inputFrame->edges->end())
		{
			result->edges.insert(std::make_pair(*thisFrameEdgeIterator, result->ADDED));
		}
		++thisFrameEdgeIterator;
	}
	inputFrameEdgeIterator = inputFrame->edges->begin();
	while (inputFrameEdgeIterator != inputFrame->edges->end())
	{
		thisFrameEdgeIterator = this->edges->getEdge(inputFrameEdgeIterator->first, inputFrameEdgeIterator->second);
		if (thisFrameEdgeIterator == this->edges->end())
		{
			result->edges.insert(std::make_pair(*inputFrameEdgeIterator, result->REMOVED));
		}
		++inputFrameEdgeIterator;
	}
}

PointsContainer::iterator Frame::pointsEnd()
{
	return points->end();
}
PointsContainer::iterator Frame::pointsBegin()
{
	return points->begin();
}
EdgesContainer::iterator Frame::edgesEnd()
{
	return edges->end();
}
EdgesContainer::iterator Frame::edgesBegin()
{
	return edges->begin();
}
