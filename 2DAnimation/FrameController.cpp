#include "FrameController.h"


FrameController::FrameController()
{
	frames.push_back(std::make_shared<Frame>());
	currentFrame = frames.begin();
}

FrameController::~FrameController()
{
	clear();
}

std::weak_ptr<Frame> FrameController::getNextFrame()
{
	(*currentFrame)->freePickedPoint();
	++currentFrame;
	if (currentFrame == frames.end())
	{
		--currentFrame;
		addFrame();
	}
	return std::weak_ptr<Frame>(*currentFrame);
}

std::weak_ptr<Frame> FrameController::getPreviousFrame()
{
	if (currentFrame != frames.begin())
	{
		(*currentFrame)->freePickedPoint();
		--currentFrame;
	}
	return std::weak_ptr<Frame>(*currentFrame);
}

std::weak_ptr<Frame> FrameController::getFirstFrame()
{
	if (currentFrame != frames.end())
	{
		(*currentFrame)->freePickedPoint();
	}
	currentFrame = frames.begin();
	return std::weak_ptr<Frame>(*currentFrame);
}

std::weak_ptr<Frame> FrameController::getLastFrame()
{
	if (currentFrame != frames.end())
	{
		(*currentFrame)->freePickedPoint();
	}
	currentFrame = --frames.end();
	return std::weak_ptr<Frame>(*currentFrame);
}

std::weak_ptr<Frame> FrameController::addFrame()
{
	(*currentFrame)->freePickedPoint();
	frames.push_back(std::make_shared<Frame>(**currentFrame));
	currentFrame = --frames.end();
	return std::weak_ptr<Frame>(*currentFrame);
}

std::weak_ptr<Frame> FrameController::addEmptyFrame()
{
	uint id;
	if (frames.size() > 0)
	{
		id = (*(--frames.end()))->getID();
	}
	else
	{
		id = 0;
	}
	frames.push_back(std::make_shared<Frame>(id));
	currentFrame = --frames.end();
	return std::weak_ptr<Frame>(*currentFrame);
}

std::weak_ptr<Frame> FrameController::getCurrentFrame()
{
	if (currentFrame == frames.end() || frames.size() == 0)
	{
		frames.push_back(std::make_shared<Frame>());
	}
	return std::weak_ptr<Frame>(*currentFrame);
}

int FrameController::totalNumberOfFrames()
{
	return frames.size();
}

void FrameController::addFrame(std::shared_ptr<Frame> frame)
{
	frames.push_back(frame);
}

void FrameController::clear()
{
	LIter it = frames.begin();
	while (it != frames.end())
	{
		frames.erase(it++);
	}
}