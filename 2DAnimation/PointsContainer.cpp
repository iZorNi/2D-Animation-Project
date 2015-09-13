#include "PointsContainer.h"



PointsContainer::PointsContainer() :
	pointNextId(0)
{}

PointsContainer::~PointsContainer()
{
}

PointsContainer::iterator PointsContainer::begin()
{
	return createIterator(points.begin());
}

PointsContainer::iterator PointsContainer::end()
{
	return createIterator(points.end());
}

PointsContainer::iterator PointsContainer::addPoint(int x, int y)
{
	MapIterator it = points.insert(std::make_pair(pointNextId, std::make_shared<Point>(x, y))).first;
	return createIterator(it);
}

PointsContainer::iterator PointsContainer::addPoint(long id, int x, int y)
{
	pointNextId = id > pointNextId ? id : pointNextId;
	return addPoint(x, y);
}

void PointsContainer::removePoint(MapIterator it)
{
	if (it != points.end())
	{
		points.erase(it);
	}
}

void PointsContainer::removePoint(long int id)
{
	MapIterator it = points.find(id);
	removePoint(it);
}

void PointsContainer::removePoint(iterator point)
{
	removePoint(point._current);
	point._current = point._end;
}

void PointsContainer::removePoint(int x, int y)
{
	MapIterator it = findPoint(x, y);
	removePoint(it);
}

PointsContainer::iterator PointsContainer::getPoint(long int id)
{
	MapIterator it = points.find(id);
	return createIterator(it);
}

PointsContainer::iterator PointsContainer::getPoint(int x, int y)
{
	MapIterator it = findPoint(x, y);
	return createIterator(it);
}

PointsContainer::iterator PointsContainer::createIterator(MapIterator it)
{
	return PointIterator(points.begin(), points.end(), it);
}

PointsContainer::MapIterator PointsContainer::findPoint(int x, int y)
{
	auto it = points.begin();
	auto end = points.end();
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

int PointsContainer::size()
{
	return points.size();
}

//////////////////////////////////
//PointIterator
//////////////////////////////////

PointsContainer::PointIterator::PointIterator(MapIterator begin, MapIterator end) :
	Iterator<std::map<long int, std::shared_ptr<Point>>>(begin, end, begin)
{}

PointsContainer::PointIterator::PointIterator(MapIterator begin, MapIterator end, MapIterator current):
	Iterator<std::map<long int, std::shared_ptr<Point>>>(begin, end, current)
{}

PointsContainer::PointIterator::PointIterator(const PointIterator& value):
	Iterator<std::map<long int, std::shared_ptr<Point>>>(value._begin, value._end, value._current)
{}

PointsContainer::PointIterator::~PointIterator()
{}

PointsContainer::PointIterator& PointsContainer::PointIterator::operator=(const PointIterator& value)
{
	return PointIterator(value);
}

Point PointsContainer::PointIterator::operator*()
{
	return *(_current->second);
}

std::shared_ptr<Point> PointsContainer::PointIterator::operator->()
{
	return _current->second;
}

/*PointsContainer::PointIterator& PointsContainer::PointIterator::operator++()
{
	if (_current != _end)
	{
		++_current;
	}
	return *this;
}

PointsContainer::PointIterator PointsContainer::PointIterator::operator++(int)
{
	auto tmp = *this;
	if (_current != _end)
	{
		++_current;
	}
	return tmp;
}

PointsContainer::PointIterator& PointsContainer::PointIterator::operator--()
{
	if (_current != _begin)
	{
		--_current;
	}
	return *this;
}

PointsContainer::PointIterator PointsContainer::PointIterator::operator--(int)
{
	auto tmp = *this;
	if (_current != _begin)
	{
		--_current;
	}
	return tmp;
}

bool PointsContainer::PointIterator::operator==(PointIterator& value)
{
	if (this->_current != value._current
		|| this->_begin != value._begin
		|| this->_end != value._end)
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool PointsContainer::PointIterator::operator!=(PointIterator& value)
{
	return !(operator==(value));
}*/

long PointsContainer::PointIterator::getId()
{
	return _current->first;
}