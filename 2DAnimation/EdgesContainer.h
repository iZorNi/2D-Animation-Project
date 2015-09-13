#pragma once
#include "stdafx.h"
#include "PointsContainer.h"
class EdgesContainer
{
public:
	class EdgeIterator;
	typedef EdgeIterator iterator;
	typedef std::set<std::pair<long int, long int>>::iterator SetIterator;
	typedef std::set<std::pair<long int, long int>>::const_iterator SetIterator_const;
private:
	std::set<std::pair<long int, long int>> edges;
	iterator createIterator(SetIterator it);
	void removeEdge(SetIterator it);
	SetIterator findEdge(int x, int y, std::weak_ptr<PointsContainer> points);
public:
	EdgesContainer();
	~EdgesContainer();
	iterator addEdge(long int a, long int b);
	void removeEdge(long int a, long int b);
	void removeEdge(int x, int y, std::weak_ptr<PointsContainer> points);
	void removeEdge(iterator edge);
	void removeEdgesWithPoint(long int a);
	iterator begin();
	iterator end();
	iterator getEdge(long int a, long int b);
	iterator getEdgeByCoord(int x, int y, std::weak_ptr<PointsContainer> points);
	int size();
};

class EdgesContainer::EdgeIterator: public Iterator<std::set<std::pair<long int, long int>>>
{
	friend class EdgesContainer;
protected:
	EdgeIterator(SetIterator begin, SetIterator end);
	EdgeIterator(SetIterator begin, SetIterator end, SetIterator current);
public:
	virtual ~EdgeIterator();
	EdgeIterator(const EdgeIterator& value);
	virtual EdgeIterator& operator=(const EdgeIterator& value);
	std::pair<long,long> operator*();
	SetIterator operator->();
};

