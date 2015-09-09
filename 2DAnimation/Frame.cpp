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
	std::shared_ptr<Point> tmp = std::make_shared<Point>(x, y);
	MIter end = points.end();
	points.insert(std::make_pair(point_next_id++, tmp));
	if (pickedPoint == end)
	{
		pickedPoint = points.end();
	}
	return point_next_id - 1;
}

long int Frame::addPoint(long int id, uint x, uint y)
{
	pickedPoint = points.end();
	std::shared_ptr<Point> tmp = std::make_shared<Point>(x, y);
	points.insert(std::make_pair(id, tmp));
	point_next_id = id >= point_next_id ? id + 1 : point_next_id;
	return id;
}

bool Frame::tryPickPoint(int x, int y)
{
	MIter it = getPointByCoord(x, y);
	MIter end = points.end();
	if (it != end && pickedPoint != end)
	{

			if (pickedPoint == it)
			{
				pickedPoint = end;
			}
			else
			{
				addEdge(pickedPoint->first, it->first);
				pickedPoint = end;
			}
	}
	else
	{
		pickedPoint = it;
	}
	return (pickedPoint != end && it!=end);
}

void Frame::movePoint(int x, int y)
{
	x = x < 0 ? 0 : x;
	y = y < 0 ? 0 : y;
	if (pickedPoint != points.end())
	{
		pickedPoint->second->setX(x);
		pickedPoint->second->setY(y);
	}
}

void Frame::removePoint()
{
	if (pickedPoint != points.end())
	{
		//delete pickedPoint->second;
		removeConnectedEdges(pickedPoint->first);
		points.erase(pickedPoint);
		pickedPoint = points.end();
	}
}

void Frame::freePoint()
{
	pickedPoint = points.end();
}

void Frame::removeConnectedEdges(long int id)
{
	SIter end = edges.end();
	SIter e = edges.begin();
	if (e != end)
	{
		while (e->first != id)
		{
			if (e->second == id)
				edges.erase(e++);
			else
				++e;
			if (e == end)
				return;
		}
	}
	if (e != end)
	{
		while (e->first == id)
		{
			edges.erase(e++);
			if (e == end)
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

void Frame::freeEdge()
{
	pickedEdge = edges.end();
}

void Frame::removeEdge()
{
	if (pickedEdge != edges.end())
	{
		edges.erase(pickedEdge);
		pickedEdge = edges.end();
	}
}

std::unique_ptr<Frame::Diff> Frame::getDiff(const Frame & frame)
{
	std::unique_ptr<Diff> res;
	res->id = this->id;
	MIter pit = this->points.begin();
	MIter_const fpit;
	while (pit != this->points.end())
	{
		fpit = frame.points.find(pit->first);
		if (fpit == frame.points.end())
		{
			res->points.insert(std::make_pair(pit->first, std::make_pair(pit->second, res->ADDED)));
		}
		else
		{
			if (fpit->second->getX() != pit->second->getX() || fpit->second->getY() != pit->second->getY())
			{
				res->points.insert(std::make_pair(pit->first, std::make_pair(pit->second, res->MOVED)));
			}
		}
		++pit;
	}

	fpit = frame.points.begin();
	while (fpit!=frame.points.end())
	{
		pit = this->points.find(fpit->first);
		if (pit == this->points.end())
		{
			res->points.insert(std::make_pair(fpit->first, std::make_pair(fpit->second, res->REMOVED)));
		}
		++fpit;
	}

	SIter eit = this->edges.begin();
	SIter_const feit;
	while (eit != this->edges.end())
	{
		feit = frame.edges.find(*eit);
		if (feit == frame.edges.end())
		{
			res->edges.insert(std::make_pair(*eit,res->ADDED));
		}
		++eit;
	}
	feit = frame.edges.begin();
	while (feit != this->edges.end())
	{
		eit = this->edges.find(*feit);
		if (eit == this->edges.end())
		{
			res->edges.insert(std::make_pair(*feit, res->REMOVED));
		}
		++feit;
	}
	return res;
}

bool Frame::pickPoint(long int id)
{
	pickedPoint = points.find(id);
	return pickedPoint != points.end();
}



bool Frame::pickEdge(long int a, long int b)
{
	if (a > b)
		std::swap(a, b);
	pickedEdge = edges.find(std::make_pair(a, b));
	return pickedEdge != edges.end();
}

int Frame::getNumberOfPoints()
{
	return points.size();
}

int Frame::getNumberOfEdges()
{
	return edges.size();
}

int Frame::getPickedPointId()
{
	long res;
	if (pickedPoint == points.end())
	{
		res = failurePoint.first;
	}
	else
	{
		res = this->pickedPoint->first;
	}
	return res;
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

std::pair<long, std::shared_ptr<Point>> Frame::resetCurrentPoint()
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

std::pair<long, long> Frame::resetCurrentEdge()
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


 
