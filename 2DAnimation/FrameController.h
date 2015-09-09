#pragma once
#include "Renderer.h"
#include "IFrameConroller.h"


class FrameController:public IFrameController
{
	std::list<std::shared_ptr<Frame>> frames;
	typedef std::list<std::shared_ptr<Frame>>::iterator LIter;
	LIter currentFrame;
public:
	FrameController();
	~FrameController();
	virtual std::weak_ptr<Frame> getNextFrame();
	virtual std::weak_ptr<Frame> getPreviousFrame();
	virtual std::weak_ptr<Frame> getFirstFrame();
	virtual std::weak_ptr<Frame> getLastFrame();
	virtual std::weak_ptr<Frame> addEmptyFrame();
	virtual std::weak_ptr<Frame> addFrame();
	virtual std::weak_ptr<Frame> getCurrentFrame();
	virtual void addFrame(std::shared_ptr<Frame> frame);
	virtual int totalNumberOfFrames();
	virtual void clear();
	//add events' processing
};

