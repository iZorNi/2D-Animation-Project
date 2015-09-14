#include "AnimationLoader.h"



AnimationLoader::~AnimationLoader()
{
}

bool AnimationLoader::loadAnimation(std::string path, std::weak_ptr<IFrameController> container)
{
	FileManager reader;
	if (!reader.openFileForReading(path))
	{
		return false;
	}
	int size;
	if (!reader.readFromFile<int>(size))
	{
		return false;
	}
	if (container.expired())
	{
		return false;
	}
	std::shared_ptr<IFrameController> currentContainer = container.lock();
	container.lock()->clear();
	std::shared_ptr<Frame> currentFrame = currentContainer->addEmptyFrame().lock();
	readFrame(currentFrame, reader);
	if (currentFrame == nullptr)
	{
		currentContainer->clear();
		return false;
	}
	for (int i = 1;i < size;++i)
	{
		currentFrame = currentContainer->addFrame().lock();
		readFrame(currentFrame,reader);
		if (currentFrame == nullptr)
		{
			currentContainer->clear();
			return false;
		}
	}
	return true;
}

std::shared_ptr<Frame> AnimationLoader::readFrame(std::shared_ptr<Frame> frame, FileManager& reader)
{
	unsigned int id;
	reader.readFromFile<unsigned int>(id);
	frame->setID(id);
	if (loadPoints(frame, reader))
	{
		loadEdges(frame, reader);
	}
	return frame;
}

AnimationLoader::dPoint AnimationLoader::readPoint(FileManager& reader)
{
	long resID = -1;
	std::shared_ptr<Point> resPoint = nullptr;
	dPoint res = std::make_pair(resID , std::make_pair( resPoint, Frame::Diff::REMOVED));
	if (!reader.readFromFile<long int>(resID))
	{
		return res;
	}
	int x, y;
	if (!reader.readFromFile<int>(x))
	{
		return res;
	}
	if (!reader.readFromFile<int>(y))
	{
		return res;
	}
	resPoint = std::make_shared<Point>(x, y);
	Frame::Diff::status resSt;
	if (!reader.readFromFile<Frame::Diff::status>(resSt))
	{
		return res;
	}
	res.first = resID;
	res.second.first = resPoint;
	res.second.second = resSt;
	return res;
}

bool AnimationLoader::loadPoints(std::shared_ptr<Frame> frame, FileManager& reader)
{
	int size;
	if (!reader.readFromFile(size))
	{
		frame = nullptr;
		return false;
	}
	for (int i = 0; i < size;++i)
	{
		if (!loadPoint(frame, readPoint(reader)))
		{
			frame = nullptr;
			break;
		}
	}
	return frame != nullptr;
}

bool AnimationLoader::loadPoint(std::shared_ptr<Frame> frame, AnimationLoader::dPoint point)
{
	bool res = (point.first != -1) && (point.second.first != nullptr);
	if (res)
	{
		int x = point.second.first->getX(), y = point.second.first->getY();
		long int id = point.first;
		Frame::Diff::status st = point.second.second;
		switch (st)
		{
		case Frame::Diff::ADDED:
			frame->addPoint(id, x, y);
			break;
		case Frame::Diff::MOVED:
			if (frame->pickPoint(id))
				frame->movePoint(x, y);
			break;
		case Frame::Diff::REMOVED:
			frame->removePoint(x,y);
			break;
		}
	}
	return res;
}

AnimationLoader::dEdge AnimationLoader::readEdge(FileManager& reader)
{
	dEdge res = std::make_pair(std::make_pair(-1, -1), Frame::Diff::REMOVED);
	long int a, b;
	if (!reader.readFromFile<long int>(a))
	{
		return res;
	}
	if (!reader.readFromFile<long int>(b))
	{
		return res;
	}
	Frame::Diff::status st;
	if (!reader.readFromFile<Frame::Diff::status>(st))
	{
		return res;
	}
	res.first.first = a;
	res.first.second = b;
	res.second = st;
	return res;
}

bool AnimationLoader::loadEdges(std::shared_ptr<Frame> frame, FileManager& reader)
{
	int size;
	if (!reader.readFromFile(size))
	{
		frame = nullptr;
		return false;
	}
	for (int i = 0; i < size;++i)
	{
		if (!loadEdge(frame, readEdge(reader)))
		{
			frame = nullptr;
			break;
		}
	}
	return frame != nullptr;
}

bool AnimationLoader::loadEdge(std::shared_ptr<Frame> frame, AnimationLoader::dEdge edge)
{
	bool res = (edge.first.first != -1) && (edge.first.second != -1);
	if (res)
	{
		long a = edge.first.first, b = edge.first.second;
		Frame::Diff::status st = edge.second;
		switch (st)
		{
		case Frame::Diff::ADDED:
			frame->addEdge(a, b);
			break;
		case Frame::Diff::MOVED:
			break;
		case Frame::Diff::REMOVED:
			frame->removeEdgeByPoints(a,b);
			break;
		default:
			break;
		}
	}
	return res;
}