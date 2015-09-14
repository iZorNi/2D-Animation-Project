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
	unsigned int id;
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
	FrameListIterator it = frames.begin();
	while (it != frames.end())
	{
		frames.erase(it++);
	}
}

std::weak_ptr<Frame> FrameController::begin()
{
	return std::weak_ptr<Frame>(*frames.begin());
}

std::weak_ptr<Frame> FrameController::end()
{
	return std::weak_ptr<Frame>();
}

FrameController::FrameIterator::FrameIterator(FrameListIterator begin, FrameListIterator end) :
	Iterator<std::list<std::shared_ptr<Frame>>>(begin, end, begin)
{}

FrameController::FrameIterator::FrameIterator(FrameListIterator begin, FrameListIterator end, FrameListIterator current) :
	Iterator<std::list<std::shared_ptr<Frame>>>(begin, end, current)
{}

FrameController::FrameIterator::FrameIterator() :
	Iterator<std::list<std::shared_ptr<Frame>>>()
{}

FrameController::FrameIterator::FrameIterator(const FrameIterator& value) :
	Iterator<std::list<std::shared_ptr<Frame>>>(value._begin, value._end, value._current)
{}

FrameController::FrameIterator& FrameController::FrameIterator::operator=(const FrameIterator& value)
{
	*this =  FrameIterator(value);
	return *this;
}

FrameController::FrameIterator::~FrameIterator()
{}

Frame FrameController::FrameIterator::operator*()
{
	return **_current;
}

std::shared_ptr<Frame> FrameController::FrameIterator::operator->()
{
	return *_current;
}