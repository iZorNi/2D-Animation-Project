#pragma once
#include "stdafx.h"

class IManager
{
public:
	virtual ~IManager() {};
	virtual bool save(std::string path) = 0;
	virtual bool load(std::string path) = 0;
	virtual void init() = 0;
	virtual void run() = 0;
	virtual void menuCall(int val, int x, int y) = 0;
	virtual void removePoint(int x, int y) = 0;
	virtual void addPoint(int x, int y) = 0;
	virtual void removeEdge(int x, int y) = 0;
};