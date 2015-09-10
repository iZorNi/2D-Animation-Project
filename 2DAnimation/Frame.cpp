#include "Frame.h"


Frame::Frame() 
{
	id = 0;
	point_next_id = 0;
	this->pickedPoint = points.end();
	this->pickedEdge =edges.end();
}

Frame::Frame(const Frame& frame)
{
	//exception not enough memory
	MIter_const mit = frame.points.begin();
	MIter_const mend = frame.points.end();
	while (mit != mend)
	{
		this->points.insert(std::make_pair(mit->first, std::make_shared<Point>(*(mit->second))));
		++mit;
	}
	
	SIter_const sit = frame.edges.begin();
	SIter_const send = frame.edges.end();
	while (sit != send)
	{
		this->edges.insert(std::pair<long int, long int>(*sit));
		++sit;
	}
	this->id = frame.id + 1;
	
	this->point_next_id = frame.point_next_id;
	
	if (frame.pickedPoint != frame.points.end())
		this->pickedPoint = this->points.find(frame.pickedPoint->first);
	else
		this->pickedPoint = this->points.end();
	
	if (frame.pickedEdge != frame.edges.end())
		this->pickedEdge = this->edges.find(*frame.pickedEdge);
	else
		this->pickedEdge = this->edges.end();
}

Frame::Frame(uint id) :id(id), point_next_id(0)
{
	pickedEdge = edges.end();
	pickedPoint = points.end();
}

Frame::~Frame()
{
	MIter it = points.begin();
	MIter_const end = points.end();
	while (it != end)
	{
		//delete it->second;
		points.erase(it++);
	}
}

uint Frame::getID()
{
	return this->id;
}

void Frame::setID(uint id)
{
	this->id = id;
}

Frame::MIter Frame::getPointByCoord(int x, int y)
{
	MIter it = points.begin();
	MIter_const end = points.end();
	while (it != end)
	{
		if (it->second->getX() == x && it->second->getY() == y)
		{
			break;
		}
		++it;
	}
	return it;
}

long int Frame::addPoint(uint x, uint y)
{
	int res = -1;
	if(!tryPickPoint(x,y))
		res =addPoint(point_next_id, x, y);
	return res;
}

long int Frame::addPoint(long int id, uint x, uint y)
{
	pickedPoint = points.end();
	points.insert(std::make_pair(id, std::make_shared<Point>(x, y)));
	point_next_id = id >= point_next_id ? id + 1 : point_next_id;
	return id;
}

bool Frame::tryPickPoint(int x, int y)
{
	auto it = getPointByCoord(x, y);
	if (it != points.end() && pickedPoint != points.end())
	{
			if (pickedPoint != it)
			{
				addEdge(pickedPoint->first, it->first);
			}
			pickedPoint = points.end();
	}
	else
	{
		pickedPoint = it;
	}
	return (pickedPoint != points.end() && it!= points.end());
}

void Frame::movePoint(int x, int y)
{
	if (pickedPoint != points.end())
	{
		x = x < 0 ? 0 : x;
		y = y < 0 ? 0 : y;
		pickedPoint->second->setX(x);
		pickedPoint->second->setY(y);
	}
}

void Frame::removePoint(int x, int y)
{
	pickedPoint = points.end();
	if (tryPickPoint(x,y))
	{
		//delete pickedPoint->second;
		removeConnectedEdges(pickedPoint->first);
		points.erase(pickedPoint);
		pickedPoint = points.end();
	}
}

void Frame::removeConnectedEdges(long int id)
{
	auto end = edges.end();
	auto edgeIterator = edges.begin();
	if (edgeIterator != end)
	{
		while (edgeIterator->first != id)
		{
			if (edgeIterator->second == id)
				edges.erase(edgeIterator++);
			else
				++edgeIterator;
			if (edgeIterator == end)
				return;
		}
	}
	if (edgeIterator != end)
	{
		while (edgeIterator->first == id)
		{
			edges.erase(edgeIterator++);
			if (edgeIterator == end)
				return;
		}
	}
}

static int getSign(int n)
{
	return (n >= 0) - (n <= 0);
}

Frame::SIter Frame::getEdgeByCoord(int x, int y)
{
	SIter sit = edges.begin();
	while (sit != edges.end())
	{
		std::shared_ptr<Point> p1 = (points.find(sit->first))->second;
		std::shared_ptr<Point> p2 = (points.find(sit->second))->second;
		bool extProdCheck = false;
		int extProd = (p2->getX() - p1->getX())*(y - p1->getY()) - (x - p1->getX())*(p2->getY() - p1->getY());
		if (extProd == 0)
		{
			extProdCheck = true;
		}
		else
		{
			int extProd1 = (p2->getX() - p1->getX())*((y - 1) - p1->getY()) - ((x - 1) - p1->getX())*(p2->getY() - p1->getY());
			int extProd2 = (p2->getX() - p1->getX())*((y + 1) - p1->getY()) - ((x + 1) - p1->getX())*(p2->getY() - p1->getY());
			extProdCheck = getSign(extProd) != getSign(extProd1) || getSign(extProd) != getSign(extProd2);
		}
		
		int dotProd = (p1->getX() - x)*(p2->getX() - x) + (p1->getY() - y)*(p2->getY() - y);
		if (extProdCheck && dotProd <= 0)
		{
			break;
		}
		++sit;
	}
	return sit;

}

bool Frame::tryPickEdge(int x, int y)
{
	pickedEdge = getEdgeByCoord(x, y);
	return (pickedEdge != edges.end());
}

bool Frame::addEdge(long int A, long int B)
{
	bool res;
	pickedEdge = edges.end();
	pickedPoint = points.end();
	if (A > B)
	{
		std::swap(A, B);
	}
	res = points.find(A) != points.end() && points.find(B) != points.end();
	if (res)
	{
		res = edges.insert(std::make_pair(A, B)).second;
	}
	return res;
}

void Frame::removeEdge(int x, int y)
{
	if (tryPickEdge(x,y))
	{
		edges.erase(pickedEdge);
		pickedEdge = edges.end();
	}
}

void Frame::removeEdgeByPoints(long int a, long int b)
{
	if (a > b)
		std::swap(a, b);
	auto tmp = edges.find(std::make_pair(a, b));
	edges.erase(tmp);
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

void Frame::getDifferenceBetweenPoints(std::shared_ptr<Frame> const inputFrame, std::unique_ptr<Frame::Diff>& result)
{
	MIter thisFramePointIterator = this->points.begin();
	MIter_const inputFramePointIterator;
	while (thisFramePointIterator != this->points.end())
	{
		inputFramePointIterator = inputFrame->points.find(thisFramePointIterator->first);
		if (inputFramePointIterator == inputFrame->points.end())
		{
			result->points.insert(std::make_pair(thisFramePointIterator->first, std::make_pair(thisFramePointIterator->second, result->ADDED)));
		}
		else
		{
			if (inputFramePointIterator->second->getX() != thisFramePointIterator->second->getX() || inputFramePointIterator->second->getY() != thisFramePointIterator->second->getY())
			{
				result->points.insert(std::make_pair(thisFramePointIterator->first, std::make_pair(thisFramePointIterator->second, result->MOVED)));
			}
		}
		++thisFramePointIterator;
	}

	inputFramePointIterator = inputFrame->points.begin();
	while (inputFramePointIterator != inputFrame->points.end())
	{
		thisFramePointIterator = this->points.find(inputFramePointIterator->first);
		if (thisFramePointIterator == this->points.end())
		{
			result->points.insert(std::make_pair(inputFramePointIterator->first, std::make_pair(inputFramePointIterator->second, result->REMOVED)));
		}
		++inputFramePointIterator;
	}
}

void Frame::getDifferenceBetweenEdges(std::shared_ptr<Frame> const inputFrame, std::unique_ptr<Frame::Diff>& result)
{
	SIter thisFrameEdgeIterator = this->edges.begin();
	SIter_const inputFrameEdgeIterator;
	while (thisFrameEdgeIterator != this->edges.end())
	{
		inputFrameEdgeIterator = inputFrame->edges.find(*thisFrameEdgeIterator);
		if (inputFrameEdgeIterator == inputFrame->edges.end())
		{
			result->edges.insert(std::make_pair(*thisFrameEdgeIterator, result->ADDED));
		}
		++thisFrameEdgeIterator;
	}
	inputFrameEdgeIterator = inputFrame->edges.begin();
	while (inputFrameEdgeIterator != inputFrame->edges.end())
	{
		thisFrameEdgeIterator = this->edges.find(*inputFrameEdgeIterator);
		if (thisFrameEdgeIterator == this->edges.end())
		{
			result->edges.insert(std::make_pair(*inputFrameEdgeIterator, result->REMOVED));
		}
		++inputFrameEdgeIterator;
	}
}

bool Frame::tryPickPoint(long int id)
{
	pickedPoint = points.find(id);
	return pickedPoint != points.end();
}

int Frame::getNumberOfPoints()
{
	return points.size();
}

int Frame::getNumberOfEdges()
{
	return edges.size();
}

std::pair<long, std::shared_ptr<Point>> Frame::getNextPoint()
{
	std::pair<long, std::shared_ptr<Point>> res; 
	if (currentPoint == points.end())
	{
		res = failurePoint;
	}
	else
	{
		res = *currentPoint;
		++currentPoint;
	}
	return res;
}

std::pair<long, std::shared_ptr<Point>> Frame::getFirstPoint()
{
	currentPoint = points.begin();
	return getNextPoint();
}

std::pair<long, std::shared_ptr<Point>> Frame::getPoint(long id)
{
	std::pair<long, std::shared_ptr<Point>> res;
	MIter p = points.find(id);
	if(p == points.end())
	{
		res = failurePoint;
	}
	else
	{
		res.first = p->first;
		res.second = p->second;
	}
	return res;
}

std::pair<long, long> Frame::getFirstEdge()
{
	currentEdge = edges.begin();
	return getNextEdge();
}

std::pair<long, long> Frame::getNextEdge()
{
	std::pair<long, long> res;
	if (currentEdge == edges.end())
	{
		res = std::make_pair(-1, -1);
	}
	else
	{
		res = *currentEdge;
		++currentEdge;
	}
	return res;
}

bool Frame::isPickedPoint(long int id)
{
	if (pickedPoint == points.end())
		return false;
	return id == pickedPoint->first;
}

//Frame::PointIterator Frame::getPointIterator(std::function<bool(std::pair<const long, std::shared_ptr<Point>>&)> qualifier)
//{
//	if (qualifier == nullptr)
//		return Frame::PointIterator(this, points.begin(), points.end());
//	else
//		return Frame::PointIterator(this, points.begin(), points.end(), qualifier);
//}
//Frame::PointIterator Frame::getPointIterator(long int id, std::function<bool(std::pair<const long, std::shared_ptr<Point>>&)> qualifier)
//{
//	if (qualifier == nullptr)
//		return Frame::PointIterator(this, points.begin(), points.end(),id);
//	else
//		return Frame::PointIterator(this, points.begin(), points.end(), id, qualifier);
//}
//Frame::PointIterator Frame::getPointIterator(int x, int y, std::function<bool(std::pair<const long, std::shared_ptr<Point>>&)> qualifier)
//{
//	if (qualifier == nullptr)
//		return Frame::PointIterator(this, points.begin(), points.end(), x, y);
//	else
//		return Frame::PointIterator(this, points.begin(), points.end(), x, y, qualifier);
//}
//
//Frame::EdgeIterator Frame::getEdgeIterator(std::function<bool(std::pair<long, long>&)> qualifier)
//{
//	if (qualifier == nullptr)
//		return Frame::EdgeIterator(this, edges.begin(), edges.end());
//	else
//		return Frame::EdgeIterator(this, edges.begin(), edges.end(), qualifier);
//}
//Frame::EdgeIterator Frame::getEdgeIterator(long int a, long int b, std::function<bool(std::pair<long, long>&)> qualifier)
//{
//	if (qualifier == nullptr)
//		return Frame::EdgeIterator(this, edges.begin(), edges.end(), a, b);
//	else
//		return Frame::EdgeIterator(this, edges.begin(), edges.end(), a, b, qualifier);
//}
//Frame::EdgeIterator Frame::getEdgeIterator(int x, int y, std::function<bool(std::pair<long, long>&)> qualifier)
//{
//	if (qualifier == nullptr)
//		return Frame::EdgeIterator(this, edges.begin(), edges.end(), x, y);
//	else
//		return Frame::EdgeIterator(this, edges.begin(), edges.end(), x, y, qualifier);
//}

void Frame::removeEdge(EdgeIterator edgeIter)
{
	auto iter = edges.find(*edgeIter);
	if (iter != edges.end())
	{
		edges.erase(iter);
	}
}
void Frame::removePoint(PointIterator pointIter)
{
	auto iter = points.find(pointIter.getId());
	if(iter != points.end())
		points.erase(iter);
}
bool Frame::tryPickPoint(PointIterator pointIter)
{
	auto iter = points.find(pointIter.getId());
	pickedPoint = iter;
	return pickedPoint != points.end();
}

bool Frame::PointIterator::allPointsQualifier(dPoint& point)
{
	return true;
}

//bool Frame::PointIterator::pointWithCoordQualifier(dPoint& point)
//{
//	return (point.second->getX() == qualifierCoordinateX && point.second->getY() == qualifierCoordinateY);
//}
//
//bool Frame::PointIterator::pointWithIdQualifier(dPoint& point)
//{
//	return point.first == qualifierID;
//}

//Frame::PointIterator::PointIterator(Frame* frame, MIter begin, MIter end, std::function<bool(dPoint&)> qualifier):
//	_begin(begin), _end(end), frame(frame)
//{
//	if (qualifier == nullptr)
//	{
//		qualifier = allPointsQualifier;
//	}
//	initCurrent();
//}
//
//Frame::PointIterator::PointIterator(Frame* frame, MIter begin, MIter end, long id, std::function<bool(dPoint&)> qualifier) :
//	_begin(begin), _end(end), frame(frame), qualifierID(id)
//{
//	if (qualifier == nullptr)
//	{
//		qualifier = pointWithIdQualifier;
//	}
//	initCurrent();
//}
//
//Frame::PointIterator::PointIterator(Frame* frame, MIter begin, MIter end, int x, int y, std::function<bool(dPoint&)> qualifier) :
//	_begin(begin), _end(end), frame(frame), qualifierCoordinateX(x), qualifierCoordinateY(y)
//{
//	if (qualifier == nullptr)
//	{
//		qualifier = pointWithCoordQualifier;
//	}
//	initCurrent();
//}

Frame::PointIterator::PointIterator(const PointIterator& value)
{
	this->_begin = value._begin;
	this->_end = value._end;
	this->_current = value._current;
	qualifierCoordinateX = value.qualifierCoordinateX;
	qualifierCoordinateY = value.qualifierCoordinateY;
	qualifierID = value.qualifierID;
}

void Frame::PointIterator::initCurrent()
{
	_current = _begin;
	while (!qualifier(*_current))
	{
		++_current;
	}
	_begin = _current;
}

Frame::PointIterator& Frame::PointIterator::setQualifierCoords(int x, int y)
{
	qualifierCoordinateX = x;
	qualifierCoordinateY = y;
	return *this;
}

Frame::PointIterator& Frame::PointIterator::setIdQualifier(long id)
{
	qualifierID = id;
	return *this;
}

Point Frame::PointIterator::operator*()
{
	return *(_current->second);
}

Frame::PointIterator& Frame::PointIterator::operator++()
{
	if (_current != _end)
	{
		do
		{
			++_current;
		} while (_current != _end && !qualifier(*_current));
	}
	return *this;
}

Frame::PointIterator Frame::PointIterator::operator++(int)
{
	auto tmp = *this;
	if (_current != _end)
	{
		do
		{
			++_current;
		} while (_current != _end && !qualifier(*_current));
	}
	return tmp;
}

Frame::PointIterator& Frame::PointIterator::operator--()
{
	if (_current != _begin)
	{
		--_current;
		while (!qualifier(*_current))
		{
			if (_current == _begin)
			{
				_current = _end;
				break;
			}
			--_current;
		}
	}
	return *this;
}

Frame::PointIterator Frame::PointIterator::operator--(int)
{
	auto tmp = *this;
	if (_current != _begin)
	{
		--_current;
		while (!qualifier(*_current))
		{
			if (_current == _begin)
			{
				_current = _end;
				break;
			}
			--_current;
		}
	}
	return tmp;
}

Frame::PointIterator& Frame::PointIterator::operator=(const Frame::PointIterator& value)
{
	this->_current = value._current;
	qualifierCoordinateX = value.qualifierCoordinateX;
	qualifierCoordinateY = value.qualifierCoordinateY;
	qualifierID = value.qualifierID;
	return *this;
}

bool Frame::PointIterator::operator==(Frame::PointIterator& value)
{

	if (   this->_current != value._current
		|| this->_begin != value._begin 
		|| this->_end != value._end)
	{
		return false;
	}
	if (   this->qualifierID != value.qualifierID 
		|| this->qualifierCoordinateX != value.qualifierCoordinateX 
		|| this->qualifierCoordinateY != value.qualifierCoordinateY)
	{
		return false;
	}
	return true;
}

bool Frame::PointIterator::operator!=(Frame::PointIterator& value)
{
	return !operator==(value);
}

Frame::PointIterator Frame::PointIterator::end()
{
	auto tmp = *this;
	tmp._current = _begin;
	return tmp;
}

Frame::PointIterator Frame::PointIterator::begin()
{
	auto tmp = *this;
	tmp._current = _begin;
	return tmp;
}

bool Frame::PointIterator::isPicked()
{
	return frame->pickedPoint->first == _current->first;
}

long Frame::PointIterator::getId()
{
	return _current->first;
}

std::shared_ptr<Point> Frame::PointIterator::operator->()
{
	return _current->second;
}

Frame::EdgeIterator::EdgeIterator(Frame* frame,SIter begin, SIter end, std::function<bool(dEdge&)> qualifier):
	_begin(begin), _end(end), qualifier(qualifier), frame(frame)
{
	initCurrent();
}
Frame::EdgeIterator::EdgeIterator(Frame* frame, SIter begin, SIter end, long a, long b, std::function<bool(dEdge&)> qualifier):
	_begin(begin), _end(end), qualifier(qualifier), frame(frame)
{
	setQualifierPoints(a, b);
	initCurrent();
}
Frame::EdgeIterator::EdgeIterator(Frame* frame, SIter begin, SIter end, int x, int y, std::function<bool(dEdge&)> qualifier):
_begin(begin), _end(end), qualifier(qualifier), frame(frame), qualifierCoordinateX(x), qualifierCoordinateY(y)
{
	initCurrent();
}

void Frame::EdgeIterator::initCurrent()
{
	_current = _begin;
	while (!qualifier(*_current))
	{
		++_current;
	}
	_begin = _current;
}

bool Frame::EdgeIterator::edgeWithPointsQualifier(dEdge& edge)
{
	return (edge.first == qualifierPointIdA && edge.second == qualifierPointIdB);
}
bool Frame::EdgeIterator::edgeByCoordQualifier(dEdge& edge)
{
	std::shared_ptr<Point> p1 = (frame->points.find(edge.first))->second;
	std::shared_ptr<Point> p2 = (frame->points.find(edge.second))->second;
	bool exteriorProductCheck = false;
	int exteriorProduct = (p2->getX() - p1->getX())*(qualifierCoordinateY - p1->getY()) - (qualifierCoordinateX - p1->getX())*(p2->getY() - p1->getY());
	if (exteriorProduct == 0)
	{
		exteriorProduct = true;
	}
	else
	{
		int exteriorProduct1 = (p2->getX() - p1->getX())*((qualifierCoordinateY - 1) - p1->getY()) - ((qualifierCoordinateX - 1) - p1->getX())*(p2->getY() - p1->getY());
		int exteriorProduct2 = (p2->getX() - p1->getX())*((qualifierCoordinateY + 1) - p1->getY()) - ((qualifierCoordinateX + 1) - p1->getX())*(p2->getY() - p1->getY());
		exteriorProductCheck = getSign(exteriorProduct) != getSign(exteriorProduct1) || getSign(exteriorProduct) != getSign(exteriorProduct2);
	}

	int dotProduct = (p1->getX() - qualifierCoordinateX)*(p2->getX() - qualifierCoordinateX) + (p1->getY() - qualifierCoordinateY)*(p2->getY() - qualifierCoordinateY);
	return exteriorProductCheck && dotProduct <= 0;
}
bool Frame::EdgeIterator::allEdgesQualifier(dEdge& edge)
{
	return true;
}

Frame::EdgeIterator& Frame::EdgeIterator::setQualifierCoords(int x, int y)
{
	qualifierCoordinateX = x;
	qualifierCoordinateY = y;
	return *this;
}
Frame::EdgeIterator& Frame::EdgeIterator::setQualifierPoints(long a, long b)
{
	qualifierPointIdA = a;
	qualifierPointIdB = b;
	return *this;
}

std::pair<long, long> Frame::EdgeIterator::operator*()
{
	return *_current;
}

Frame::SIter Frame::EdgeIterator::operator->()
{
	return _current;
}

Frame::EdgeIterator& Frame::EdgeIterator::operator++()
{
	if (_current != _end)
	{
		do
		{
			++_current;
		} while (_current != _end && !qualifier(*_current));
	}
	return *this;
}

Frame::EdgeIterator Frame::EdgeIterator::operator++(int)
{
	auto tmp = *this;
	if (_current != _end)
	{
		do
		{
			++_current;
		} while (_current != _end && !qualifier(*_current));
	}
	return tmp;
}

Frame::EdgeIterator& Frame::EdgeIterator::operator--()
{
	if (_current != _begin)
	{
		--_current;
		while (!qualifier(*_current))
		{
			if (_current == _begin)
			{
				_current = _end;
				break;
			}
			--_current;
		}
	}
	return *this;
}

Frame::EdgeIterator Frame::EdgeIterator::operator--(int)
{
	auto tmp = *this;
	if (_current != _begin)
	{
		--_current;
		while (!qualifier(*_current))
		{
			if (_current == _begin)
			{
				_current = _end;
				break;
			}
			--_current;
		}
	}
	return tmp;
}

bool Frame::EdgeIterator::operator==(Frame::EdgeIterator& value)
{

	if (this->_current != value._current
		|| this->_begin != value._begin
		|| this->_end != value._end)
	{
		return false;
	}
	if (this->qualifierPointIdA != value.qualifierPointIdA
		|| this->qualifierPointIdB != value.qualifierPointIdB
		|| this->qualifierCoordinateX != value.qualifierCoordinateX
		|| this->qualifierCoordinateY != value.qualifierCoordinateY)
	{
		return false;
	}
	return true;
}

bool Frame::EdgeIterator::operator!=(Frame::EdgeIterator& value)
{
	return !operator==(value);
}

Frame::EdgeIterator& Frame::EdgeIterator::operator=(const EdgeIterator& value)
{
	this->qualifierPointIdA = value.qualifierPointIdA;
	this->qualifierPointIdB = value.qualifierPointIdB;
	this->qualifierCoordinateX = value.qualifierCoordinateX;
	this->qualifierCoordinateY = value.qualifierCoordinateY;
	this->_current = value._current;
	return *this;
}

Frame::EdgeIterator Frame::EdgeIterator::end()
{
	auto tmp = *this;
	tmp._current = _end;
	return tmp;
}

Frame::EdgeIterator Frame::EdgeIterator::begin()
{
	auto tmp = *this;
	tmp._current = _begin;
	return tmp;
}

std::pair<std::pair<int, int>, std::pair<int, int>> Frame::EdgeIterator::getPointsCoord()
{
	int x1 = frame->points.find(_current->first)->second->getX();
	int y1 = frame->points.find(_current->first)->second->getY();
	int x2 = frame->points.find(_current->second)->second->getX();
	int y2 = frame->points.find(_current->second)->second->getY();
	std::pair<int, int> firstPair = std::make_pair(x1, y1);
	std::pair<int, int> secondPair = std::make_pair(x2, y2);
	return std::make_pair(firstPair, secondPair);
}
