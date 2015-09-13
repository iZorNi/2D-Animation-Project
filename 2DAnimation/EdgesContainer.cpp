#include "EdgesContainer.h"



EdgesContainer::EdgesContainer()
{
}


EdgesContainer::~EdgesContainer()
{
}

EdgesContainer::iterator EdgesContainer::createIterator(SetIterator it)
{
	return EdgeIterator(edges.begin(), edges.end(), it);
}

EdgesContainer::iterator EdgesContainer::begin()
{
	return createIterator(edges.begin());
}

EdgesContainer::iterator EdgesContainer::end()
{
	return createIterator(edges.end());
}

void EdgesContainer::removeEdge(SetIterator it)
{
	if (it != edges.end())
	{
		edges.erase(it);
	}
}

void EdgesContainer::removeEdge(long int a, long int b)
{
	SetIterator it = edges.find(std::make_pair(a, b));
	removeEdge(it);
}

void EdgesContainer::removeEdge(int x, int y, std::weak_ptr<PointsContainer> points)
{
	SetIterator it = findEdge(x, y, points);
	removeEdge(it);
}

void EdgesContainer::removeEdge(iterator edge)
{
	removeEdge(edge._current);
}

void EdgesContainer::removeEdgesWithPoint(long int id)
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

EdgesContainer::iterator EdgesContainer::addEdge(long int a, long int b)
{
	SetIterator it = edges.insert(std::make_pair(a, b)).first;
	return createIterator(it);
}

namespace {
	static int getSign(int n)
	{
		return (n >= 0) - (n <= 0);
	}
}

EdgesContainer::SetIterator EdgesContainer::findEdge(int x, int y, std::weak_ptr<PointsContainer> points)
{
	auto iter = edges.begin();
	std::shared_ptr<PointsContainer> sPoints = points.lock();
	while (iter != edges.end())
	{
		Point p1 = *(sPoints->getPoint(iter->first));
		Point p2 = *(sPoints->getPoint(iter->second));
		bool extProdCheck = false;
		int extProd = (p2.getX() - p1.getX())*(y - p1.getY()) - (x - p1.getX())*(p2.getY() - p1.getY());
		if (extProd == 0)
		{
			extProdCheck = true;
		}
		else
		{
			int extProd1 = (p2.getX() - p1.getX())*((y - 1) - p1.getY()) - ((x - 1) - p1.getX())*(p2.getY() - p1.getY());
			int extProd2 = (p2.getX() - p1.getX())*((y + 1) - p1.getY()) - ((x + 1) - p1.getX())*(p2.getY() - p1.getY());
			extProdCheck = getSign(extProd) != getSign(extProd1) || getSign(extProd) != getSign(extProd2);
		}

		int dotProd = (p1.getX() - x)*(p2.getX() - x) + (p1.getY() - y)*(p2.getY() - y);
		if (extProdCheck && dotProd <= 0)
		{
			break;
		}
		++iter;
	}
}

EdgesContainer::iterator EdgesContainer::getEdge(long int a, long int b)
{
	SetIterator it = edges.find(std::make_pair(a,b));
	return createIterator(it);
}

EdgesContainer::iterator EdgesContainer::getEdgeByCoord(int x, int y, std::weak_ptr<PointsContainer> points)
{
	SetIterator it = findEdge(x, y, points);
	return createIterator(it);
}

int EdgesContainer::size()
{
	return edges.size();
}

/////////////////////////////////
//EdgeIterator
////////////////////////////////

EdgesContainer::EdgeIterator::EdgeIterator(SetIterator begin, SetIterator end) :
	Iterator<std::set<std::pair<long int, long int>>>(begin, end, begin)
{}

EdgesContainer::EdgeIterator::EdgeIterator(SetIterator begin, SetIterator end, SetIterator current) :
	Iterator<std::set<std::pair<long int, long int>>>(begin, end, current)
{}

EdgesContainer::EdgeIterator::EdgeIterator(const EdgeIterator& value):
	Iterator<std::set<std::pair<long int, long int>>>(value._begin, value._end, value._current)
{}

EdgesContainer::EdgeIterator::~EdgeIterator()
{}

EdgesContainer::EdgeIterator& EdgesContainer::EdgeIterator::operator=(const EdgeIterator& value)
{
	return EdgeIterator(value);
}

std::pair<long, long> EdgesContainer::EdgeIterator::operator*()
{
	return *_current;
}

EdgesContainer::SetIterator EdgesContainer::EdgeIterator::operator->()
{
	return _current;
}
