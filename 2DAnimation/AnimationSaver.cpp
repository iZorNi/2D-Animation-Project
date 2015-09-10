#include "AnimationSaver.h"



AnimationSaver::AnimationSaver():path("")
{
}

AnimationSaver::~AnimationSaver()
{
}

AnimationSaver::AnimationSaver(std::string path) :path(path)
{
};

bool AnimationSaver::saveAnimation(std::weak_ptr<IFrameController> container)
{
	if (!path.length() != 0)
	{
		return false;
	}
	FileManager writer;
	if (!writer.openFileForWriting(path))
	{
		return false;
	}
	std::shared_ptr<IFrameController> currentContainer;
	if(container.expired())
	{
		return false;
	}
	currentContainer = container.lock();
	if (!writer.writeToFile<int>(currentContainer->totalNumberOfFrames()))
	{
		return false;
	}
	//Fake-zero frame for algorithnm to be more consistent
	int total = currentContainer->totalNumberOfFrames();
	std::shared_ptr<Frame> fakeZeroShared = std::make_shared<Frame>(total);
	std::weak_ptr<Frame>fakezero = std::weak_ptr<Frame>(fakeZeroShared);
	std::weak_ptr<Frame> currentFrame = currentContainer->getFirstFrame();
	if(!saveDifference(currentFrame.lock()->getDifference(fakezero), writer))
	{
		return false;
	}
	fakeZeroShared.reset();
	for (int i = 1; i < total; ++i)
	{
		auto tmpFrame = currentContainer->getNextFrame().lock();
		std::unique_ptr<Frame::Diff> diff = tmpFrame->getDifference(currentFrame);
		if (!saveDifference(diff, writer))
		{	
			return false;
		}
		currentFrame = currentContainer->getCurrentFrame();
	}
	writer.closeOutputFile();
	return true;
}

void AnimationSaver::setPath(std::string path)
{
	this->path = path;
}

bool AnimationSaver::saveFrame(std::shared_ptr<Frame> frame, FileManager& writer)
{
	bool res;
	res = saveFrameID(frame->getID(),writer);
	if (res)
	{
		res = savePoints(frame, writer);
	}
	if (res)
	{
		res = saveEdges(frame, writer);
	}
	return res;
}

bool AnimationSaver::saveFrameID(unsigned int id, FileManager& writer)
{
	return writer.writeToFile<unsigned int>(id);
}

bool AnimationSaver::savePoints(std::shared_ptr<Frame> frame, FileManager& writer)
{
	if (!writer.writeToFile<int>(frame->getNumberOfPoints()))
	{
		return false;
	}
	std::pair<long int, std::shared_ptr<Point>> p = frame->getFirstPoint();
	while (p != frame->failurePoint)
	{
		if (!savePoint(p.first, p.second->getX(), p.second->getY(), Frame::Diff::ADDED, writer))
		{
			return false;
		}
		p = frame->getNextPoint();
	}
	return true;
}

bool AnimationSaver::saveEdges(std::shared_ptr<Frame> frame, FileManager& writer)
{
	if (!writer.writeToFile<unsigned int>(frame->getNumberOfEdges()))
	{
		return false;
	}
	std::pair<long int, long int> edgeIter = frame->getFirstEdge();
	while (edgeIter.first != -1 && edgeIter.second != -1)
	{
		if (!saveEdge(edgeIter.first, edgeIter.second, Frame::Diff::ADDED,writer))
			return false;
		edgeIter = frame->getNextEdge();
	}
	return true;
}

bool AnimationSaver::saveDifference(std::unique_ptr<Frame::Diff>& diff, FileManager& writer)
{
	bool res;
	res = writer.writeToFile<unsigned int>(diff->id);
	//points
	if (res)
	{
		res = saveDifferencePoints(diff, writer);
	}
	//edges
	if (res)
	{
		res = saveDifferenceEdges(diff, writer);
	}
	return res;
}

bool AnimationSaver::saveDifferencePoints(std::unique_ptr<Frame::Diff>& diff, FileManager& writer)
{
	if (!writer.writeToFile<unsigned int>(diff->points.size()))
	{
		return false;
	}
	std::map<long int, std::pair<std::shared_ptr<Point>, Frame::Diff::status>>::iterator pointIterator = diff->points.begin();
	while (pointIterator != diff->points.end())
	{
		if (!savePoint(pointIterator->first, pointIterator->second.first->getX(), pointIterator->second.first->getY(), pointIterator->second.second, writer))
		{
			return false;
		}
		++pointIterator;
	}
	return true;
}

bool AnimationSaver::saveDifferenceEdges(std::unique_ptr<Frame::Diff>& diff, FileManager& writer)
{
	if (!writer.writeToFile<unsigned int>(diff->edges.size()))
	{
		return false;
	}
	std::set<std::pair<std::pair<long int, long int>, Frame::Diff::status>>::iterator edgeIter = diff->edges.begin();
	while (edgeIter != diff->edges.end())
	{
		if (!saveEdge(edgeIter->first.first, edgeIter->first.second, edgeIter->second,writer))
		{
			return false;
		}
		++edgeIter;
	}
	return true;
}

bool AnimationSaver::savePoint(long int id, int x, int y, Frame::Diff::status status, FileManager& writer)
{
	bool res;
	res = writer.writeToFile<long int>(id);
	if (!res) return res;
	res = writer.writeToFile<int>(x);
	if (!res) return res;
	res = writer.writeToFile<int>(y);
	if (!res) return res;
	res = writer.writeToFile<Frame::Diff::status>(status);
	if (!res) return res;
	return res;
}

bool AnimationSaver::saveEdge(long int a, long int b, Frame::Diff::status status, FileManager& writer)
{
	bool res;
	res = writer.writeToFile<long int>(a);
	if (!res) return res;
	res = writer.writeToFile<long int>(b);
	if (!res) return res;
	res = writer.writeToFile<Frame::Diff::status>(status);
	if (!res) return res;
	return res;
}