#pragma once
#include "Frame.h"

class IFrameController
{
public:
	virtual ~IFrameController() {}
	virtual std::weak_ptr<Frame> getNextFrame() = 0;
	virtual std::weak_ptr<Frame> getPreviousFrame() = 0;
	virtual std::weak_ptr<Frame> getFirstFrame() = 0;
	virtual std::weak_ptr<Frame> getLastFrame() = 0;
	virtual std::weak_ptr<Frame> addEmptyFrame() = 0;
	virtual std::weak_ptr<Frame> addFrame() = 0;
	virtual std::weak_ptr<Frame> getCurrentFrame() = 0;
	virtual std::weak_ptr<Frame> begin() = 0;
	virtual std::weak_ptr<Frame> end() = 0;
	virtual void addFrame(std::shared_ptr<Frame> frame) = 0;
	virtual int totalNumberOfFrames() = 0;
	virtual void clear() = 0;
};

