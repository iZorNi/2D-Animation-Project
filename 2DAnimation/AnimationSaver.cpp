#include "AnimationSaver.h"



AnimationSaver::~AnimationSaver()
{
}

bool AnimationSaver::saveAnimation(std::string path, std::weak_ptr<IFrameController> container)
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
	int total = currentContainer->totalNumberOfFrames();
	if (!writer.writeToFile<int>(total))
	{
		return false;
	}

	//Fake-zero frame for algorithnm to be more consistent
	std::shared_ptr<Frame> fakeZeroShared = std::make_shared<Frame>(total);
	std::weak_ptr<Frame>fakezero = std::weak_ptr<Frame>(fakeZeroShared);
	std::weak_ptr<Frame> currentFrame = currentContainer->getFirstFrame();
	
	//save difference between first and empty frame
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
		long id = pointIterator->first;
		int x = pointIterator->second.first->getX();
		int y = pointIterator->second.first->getY();
		Frame::Diff::status status = pointIterator->second.second;
		if (!savePoint(id, x, y, status, writer))
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