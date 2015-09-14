#pragma once
#include "Renderer.h"
#include "IFrameConroller.h"


class FrameController:public IFrameController
{
public:
	class FrameIterator;
	typedef FrameIterator iterator;
	typedef std::list<std::shared_ptr<Frame>>::iterator FrameListIterator;
private:
	std::list<std::shared_ptr<Frame>> frames;
	FrameListIterator currentFrame;
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
	virtual std::weak_ptr<Frame> begin();
	virtual std::weak_ptr<Frame> end();
	virtual void addFrame(std::shared_ptr<Frame> frame);
	virtual int totalNumberOfFrames();
	virtual void clear();
	//add events' processing
};

class FrameController::FrameIterator :Iterator<std::list<std::shared_ptr<Frame>>>
{
	friend class FrameController;
protected:
	FrameIterator(FrameListIterator begin, FrameListIterator end);
	FrameIterator(FrameListIterator begin, FrameListIterator end, FrameListIterator current);
public:
	FrameIterator();
	virtual ~FrameIterator();
	FrameIterator(const FrameIterator& value);
	virtual FrameIterator& operator=(const FrameIterator& value);
	Frame operator*();
	std::shared_ptr<Frame> operator->();

};