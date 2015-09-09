#include "FrameContainer.h"


FrameContainer::FrameContainer()
{
	frames.push_back(std::make_shared<Frame>());
	currentFrame = frames.begin();
}

FrameContainer::~FrameContainer()
{
	clear();
}

std::shared_ptr<Frame> FrameContainer::next()
{
	(*currentFrame)->freePoint();
	(*currentFrame)->freeEdge();
	++currentFrame;
	if (currentFrame == frames.end())
	{
		--currentFrame;
		add();
	}
	return *currentFrame;
}

std::shared_ptr<Frame> FrameContainer::prev()
{
	if (currentFrame != frames.begin())
	{
		(*currentFrame)->freePoint();
		(*currentFrame)->freeEdge();
		--currentFrame;
	}
	return *currentFrame;
}

std::shared_ptr<Frame> FrameContainer::begin()
{
	currentFrame = frames.begin();
	return *currentFrame;
}

std::shared_ptr<Frame> FrameContainer::last()
{
	currentFrame = --frames.end();
	return *currentFrame;
}

std::shared_ptr<Frame> FrameContainer::add()
{
	(*currentFrame)->freePoint();
	(*currentFrame)->freeEdge();
	frames.push_back(std::make_shared<Frame>(**currentFrame));
	currentFrame = --frames.end();
	return *currentFrame;
}

std::shared_ptr<Frame> FrameContainer::addEmpty()
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
	return *currentFrame;
}

std::shared_ptr<Frame> FrameContainer::getCurrent()
{
	if (currentFrame == frames.end() || frames.size() == 0)
	{
		frames.push_back(std::make_shared<Frame>());
	}
	return *currentFrame;
}

int FrameContainer::total()
{
	return frames.size();
}

void FrameContainer::addFrame(std::shared_ptr<Frame> frame)
{
	frames.push_back(frame);
}

void FrameContainer::clear()
{
	LIter it = frames.begin();
	while (it != frames.end())
	{
		frames.erase(it++);
	}
}