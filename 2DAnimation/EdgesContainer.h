#pragma once
#include "stdafx.h"
#include "PointsContainer.h"
class EdgesContainer
{
public:
	class EdgeIterator;
	typedef EdgeIterator iterator;
	typedef std::set<std::pair<long int, long int>>::iterator EdgesSetIterator;
	typedef std::set<std::pair<long int, long int>>::const_iterator EdgesSetIterator_const;
private:
	std::set<std::pair<long int, long int>> edges;
	iterator createIterator(EdgesSetIterator it);
	void removeEdge(EdgesSetIterator it);
	EdgesSetIterator findEdge(int x, int y, std::weak_ptr<PointsContainer> points);
public:
	EdgesContainer();
	~EdgesContainer();
	iterator addEdge(long int a, long int b);
	iterator copyEdge(iterator edgeIterator);
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
	EdgeIterator(EdgesSetIterator begin, EdgesSetIterator end);
	EdgeIterator(EdgesSetIterator begin, EdgesSetIterator end, EdgesSetIterator current);
public:
	EdgeIterator();
	virtual ~EdgeIterator();
	EdgeIterator(const EdgeIterator& value);
	virtual EdgeIterator& operator=(const EdgeIterator& value);
	std::pair<long,long> operator*();
	EdgesSetIterator operator->();
};

