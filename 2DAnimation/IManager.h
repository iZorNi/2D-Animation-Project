#pragma once
#include "Frame.h"

class IManager
{
public:
	virtual ~IManager() {};
	virtual bool save(std::string path) = 0;
	virtual bool load(std::string path) = 0;
	virtual void menuCall(int val, int x, int y) = 0;
	virtual void removePoint(int x, int y) = 0;
	virtual void addPoint(int x, int y) = 0;
	virtual void removeEdge(int x, int y) = 0;
	virtual void movePoint(int x, int y) = 0;
	virtual void pickPoint(int x, int y) = 0;
	virtual void nextFrame() = 0;
	virtual void previousFrame() = 0;
	virtual std::weak_ptr<Frame> getCurrentFrame() = 0;
private:
};