#pragma once
#include "Renderer.h"
#include "IFrameContainer.h"


class FrameContainer:public IFrameContainer
{
	std::list<std::shared_ptr<Frame>> frames;
	typedef std::list<std::shared_ptr<Frame>>::iterator LIter;
	LIter currentFrame;
public:
	FrameContainer();
	~FrameContainer();
	virtual std::shared_ptr<Frame> next();
	virtual std::shared_ptr<Frame> prev();
	virtual std::shared_ptr<Frame> begin();
	virtual std::shared_ptr<Frame> last();
	virtual std::shared_ptr<Frame> addEmpty();
	virtual std::shared_ptr<Frame> add();
	virtual std::shared_ptr<Frame> getCurrent();
	virtual void addFrame(std::shared_ptr<Frame> frame);
	virtual int total();
	virtual void clear();
	//add events' processing
};

