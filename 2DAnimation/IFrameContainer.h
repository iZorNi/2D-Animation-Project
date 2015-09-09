#pragma once
#include "Frame.h"

class IFrameContainer
{
protected:
	//typedef std::list<std::shared_ptr<T>>::iterator LIter;
public:
	virtual ~IFrameContainer() {}
	virtual std::shared_ptr<Frame> next() = 0;
	virtual std::shared_ptr<Frame> prev() = 0;
	virtual std::shared_ptr<Frame> begin() = 0;
	virtual std::shared_ptr<Frame> last() = 0;
	virtual std::shared_ptr<Frame> addEmpty() = 0;
	virtual std::shared_ptr<Frame> add() = 0;
	virtual std::shared_ptr<Frame> getCurrent() = 0;
	virtual void addFrame(std::shared_ptr<Frame> frame) = 0;
	virtual void clear() = 0;
	virtual int total() = 0;
};

