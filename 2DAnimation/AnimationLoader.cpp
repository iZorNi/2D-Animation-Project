#include "AnimationLoader.h"



AnimationLoader::AnimationLoader()
{
}

AnimationLoader::~AnimationLoader()
{
}

void AnimationLoader::setPath(std::string path)
{
	this->path = path;
}

bool AnimationLoader::loadAnimation(std::weak_ptr<IFrameController> container)
{
	bool res = fm.openFileForReading(path);
	if (res)
	{
		int size;
		res = fm.readFromFile<int>(size);
		if (!res)
		{
			fm.closeInputFile();
			return res;
		}
		container.lock()->clear();
		readFrame(container.lock()->addEmptyFrame().lock());
		if (container.lock()->getCurrentFrame().lock() == nullptr)
		{
			container.lock()->clear();
			fm.closeInputFile();
			res = false;
		}
		else
		{
			for (int i = 1;i < size;++i)
			{
				readFrame(container.lock()->addFrame().lock());
				if (container.lock()->getCurrentFrame().lock() == nullptr)
				{
					container.lock()->clear();
					fm.closeInputFile();
					res = false;
					break;
				}
			}
		}
	}
	fm.closeInputFile();
	return res;
}

std::shared_ptr<Frame> AnimationLoader::readFrame(std::shared_ptr<Frame> frame)
{
	unsigned int id;
	fm.readFromFile<unsigned int>(id);
	frame->setID(id);
	if (loadPoints(frame))
	{
		loadEdges(frame);
	}
	return frame;
}

AnimationLoader::dPoint AnimationLoader::readPoint()
{
	bool success;
	long resID = -1;
	std::shared_ptr<Point> resPoint = nullptr;
	dPoint res = std::make_pair(resID , std::make_pair( resPoint, Frame::Diff::REMOVED));
	success = fm.readFromFile<long int>(resID);
	if (!success) return res;
	int x, y;
	success = fm.readFromFile<int>(x);
	if (!success) return res;
	success = fm.readFromFile<int>(y);
	if (!success) return res;
	resPoint = std::make_shared<Point>(x, y);
	Frame::Diff::status resSt;
	success = fm.readFromFile<Frame::Diff::status>(resSt);
	if (!success) return res;
	res.first = resID;
	res.second.first = resPoint;
	res.second.second = resSt;
	return res;
}

bool AnimationLoader::loadPoints(std::shared_ptr<Frame> frame)
{
	int size;
	if (!fm.readFromFile(size))
		frame = nullptr;
	else
	{
		for (int i = 0; i < size;++i)
		{
			if (!loadPoint(frame, readPoint()))
			{
				frame = nullptr;
				break;
			}
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
			if (frame->tryPickPoint(id))
				frame->movePoint(x, y);
			break;
		case Frame::Diff::REMOVED:
			frame->removePoint(x,y);
			break;
		}
	}
	return res;
}

AnimationLoader::dEdge AnimationLoader::readEdge()
{
	bool success;
	dEdge res = std::make_pair(std::make_pair(-1, -1), Frame::Diff::REMOVED);
	long int a, b;
	success = fm.readFromFile<long int>(a);
	if (!success) return res;
	success = fm.readFromFile<long int>(b);
	if (!success) return res;
	Frame::Diff::status st;
	success = fm.readFromFile<Frame::Diff::status>(st);
	if (!success) return res;
	res.first.first = a;
	res.first.second = b;
	res.second = st;
	return res;
}

bool AnimationLoader::loadEdges(std::shared_ptr<Frame> frame)
{
	int size;
	if (!fm.readFromFile(size))
		frame = nullptr;
	else
	{
		for (int i = 0; i < size;++i)
		{
			if (!loadEdge(frame, readEdge()))
			{
				frame = nullptr;
				break;
			}
		}
	}
	return frame != nullptr;
}

bool AnimationLoader::loadEdge(std::shared_ptr<Frame> frame, AnimationLoader::dEdge edge)
{
	bool res = (edge.first.first != -1) && (edge.first.second != -1);
	if (res)
	{
		int a = edge.first.first, b = edge.first.second;
		Frame::Diff::status st = edge.second;
		switch (st)
		{
		case Frame::Diff::ADDED:
			res = frame->addEdge(a, b);
			break;
		case Frame::Diff::MOVED:
			break;
		case Frame::Diff::REMOVED:
			frame->removeEdge(a,b);
			break;
		default:
			break;
		}
	}
	return res;
}