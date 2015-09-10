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

std::weak_ptr<Frame> FrameController::begin()
{
	return std::weak_ptr<Frame>(*frames.begin());
}
std::weak_ptr<Frame> FrameController::end()
{
	return std::weak_ptr<Frame>();
}

FrameController::FrameIterator::FrameIterator(LIter begin, LIter end, std::function<bool(std::shared_ptr<Frame>)> qualifier)
	: _begin(begin), _end(end), qualifier(qualifier)
{
}
FrameController::FrameIterator::FrameIterator(LIter begin, LIter end, int id, std::function<bool(std::shared_ptr<Frame>)> qualifier)
	: _begin(begin), _end(end), qualifier(qualifier), qualifierID(id)
{
}
void FrameController::FrameIterator::initCurrent()
{
	_current = _begin;
	while (!qualifier(*_current))
	{
		++_current;
	}
}
bool FrameController::FrameIterator::frameWithIdQualifier(std::shared_ptr<Frame> frame)
{
	return true;
}
bool FrameController::FrameIterator::allFramesQualifier(std::shared_ptr<Frame> frame)
{
	return frame->getID() == qualifierID;
}

std::shared_ptr<Frame> FrameController::FrameIterator::operator*()
{
	return *(_current);
}
std::shared_ptr<Frame> FrameController::FrameIterator::operator->()
{
	return *(_current);
}
FrameController::FrameIterator& FrameController::FrameIterator::operator++()
{
	if (_current != _end)
	{
		do
		{
			++_current;
		} while (_current != _end && !qualifier(*_current));
	}
	return *this;
}
FrameController::FrameIterator FrameController::FrameIterator::operator++(int)
{
	auto tmp = *this;
	if (_current != _end)
	{
		do
		{
			++_current;
		} while (_current != _end && !qualifier(*_current));
	}
	return tmp;
}
FrameController::FrameIterator& FrameController::FrameIterator::operator--()
{
	if (_current != _begin)
	{
		--_current;
		while (!qualifier(*_current))
		{
			if (_current == _begin)
			{
				_current = _end;
				break;
			}
			--_current;
		}
	}
	return *this;
}
FrameController::FrameIterator FrameController::FrameIterator::operator--(int)
{
	auto tmp = *this;
	if (_current != _begin)
	{
		--_current;
		while (!qualifier(*_current))
		{
			if (_current == _begin)
			{
				_current = _end;
				break;
			}
			--_current;
		}
	}
	return tmp;
}
FrameController::FrameIterator& FrameController::FrameIterator::operator=(const FrameIterator& value)
{
	this->qualifierID = value.qualifierID;
	this->_current = value._current;
	return *this;
}
bool FrameController::FrameIterator::operator==(FrameIterator& value)
{
	if (_current != value._current || _end != value._end
		|| _begin != value._begin  || qualifierID !=value.qualifierID)
	{
		return false;
	}
	return true;
}
bool FrameController::FrameIterator::operator!=(FrameIterator& value)
{
	return !operator==(value);
}
const FrameController::FrameIterator FrameController::FrameIterator::end()
{
	auto tmp = *this;
	tmp._current = _begin;
	return tmp;
}
const FrameController::FrameIterator FrameController::FrameIterator::begin()
{
	auto tmp = *this;
	tmp._current = _begin;
	return tmp;
}