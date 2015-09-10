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

	class FrameIterator
	{
		LIter _begin;
		LIter _end;
		LIter _current;
		std::function<bool(std::shared_ptr<Frame>)> qualifier;
		typedef std::list<std::shared_ptr<Frame>>::iterator LIter;
		int qualifierID;
		void initCurrent();
	public:
		FrameIterator(LIter begin, LIter end, std::function<bool(std::shared_ptr<Frame>)> qualifier = &allFramesQualifier);
		FrameIterator(LIter begin, LIter end, int id, std::function<bool(std::shared_ptr<Frame>)> qualifier = &frameWithIdQualifier);
		bool frameWithIdQualifier(std::shared_ptr<Frame> frame);
		bool allFramesQualifier(std::shared_ptr<Frame> frame);
		std::shared_ptr<Frame> operator*();
		std::shared_ptr<Frame> operator->();
		FrameIterator& operator++();
		FrameIterator operator++(int);
		FrameIterator& operator--();
		FrameIterator operator--(int);
		virtual FrameIterator& operator=(const FrameIterator& value);
		virtual ~FrameIterator() {};
		const FrameIterator end();
		const FrameIterator begin();
	};
};

