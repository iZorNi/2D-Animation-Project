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
	bool res = path.length() != 0;
	if (res)
	{
		res = fm.openFileForWriting(path);
		if (res)
		{
			if (fm.writeToFile<int>(container.lock()->totalNumberOfFrames()))
			{
				std::weak_ptr<Frame> f = container.lock()->getFirstFrame();
				res = saveDifference(f.lock()->getDiff(std::weak_ptr<Frame>(std::make_shared<Frame>(container.lock()->totalNumberOfFrames()))));
				if (!res)
				{
					fm.closeOutputFile();
					return res;
				}
				while (f.lock() != (container.lock()->getLastFrame().lock()))
				{
					std::unique_ptr<Frame::Diff> diff = container.lock()->getNextFrame().lock()->getDiff(f);
					if (!saveDifference(diff))
					{	
						res = false;
						break;
					}
					f = container.lock()->getCurrentFrame();
				}
			}
			else
			{
				res = false;
			}
			fm.closeOutputFile();
		}
	}
	return res;
}

void AnimationSaver::setPath(std::string path)
{
	this->path = path;
}

bool AnimationSaver::saveFrame(std::shared_ptr<Frame> frame)
{
	bool res;
	res = saveFrameID(frame->getID());
	if (res)
	{
		res = savePoints(frame);
	}
	if (res)
	{
		res = saveEdges(frame);
	}
	return res;
}

bool AnimationSaver::saveFrameID(unsigned int id)
{
	return fm.writeToFile<unsigned int>(id);
}

bool AnimationSaver::savePoints(std::shared_ptr<Frame> frame)
{
	bool result = true;
	if (fm.writeToFile<int>(frame->getNumberOfPoints()))
	{
		std::pair<long int, std::shared_ptr<Point>> p = frame->resetCurrentPoint();
		while (p != frame->failurePoint)
		{
			if (!savePoint(p.first, p.second->getX(), p.second->getY(), Frame::Diff::ADDED))
			{
				result = false;
				break;
			}
			p = frame->getNextPoint();
		}
	}
	else
	{
		result = false;
	}
	return result;
}

bool AnimationSaver::saveEdges(std::shared_ptr<Frame> frame)
{
	bool result;
	result = fm.writeToFile<unsigned int>(frame->getNumberOfEdges());
	if (result)
	{
		std::pair<long int, long int> e = frame->resetCurrentEdge();
		while (e.first != -1 && e.second != -1)
		{
			result = saveEdge(e.first, e.second, Frame::Diff::ADDED);
			if (!result)
				break;
			e = frame->getNextEdge();
		}
	}
	return result;
}

bool AnimationSaver::saveDifference(std::unique_ptr<Frame::Diff>& diff)
{
	bool res;
	res = fm.writeToFile<unsigned int>(diff->id);
	//points
	if (res)
	{
		res = saveDifferencePoints(diff);
	}
	//edges
	if (res)
	{
		res = saveDifferenceEdges(diff);
	}
	return res;
}

bool AnimationSaver::saveDifferencePoints(std::unique_ptr<Frame::Diff>& diff)
{
	bool result = true;
	if (fm.writeToFile<unsigned int>(diff->points.size()))
	{
		std::map<long int, std::pair<std::shared_ptr<Point>, Frame::Diff::status>>::iterator pit = diff->points.begin();
		while (pit != diff->points.end())
		{
			if (!savePoint(pit->first, pit->second.first->getX(), pit->second.first->getY(), pit->second.second))
			{
				result = false;
				break;
			}
		}
	}
	else
	{
		result = false;
	}
	return result;
}

bool AnimationSaver::saveDifferenceEdges(std::unique_ptr<Frame::Diff>& diff)
{
	bool result = true;
	if (fm.writeToFile<unsigned int>(diff->edges.size()))
	{
		std::set<std::pair<std::pair<long int, long int>, Frame::Diff::status>>::iterator eit = diff->edges.begin();
		while (eit != diff->edges.end())
		{
			if (!saveEdge(eit->first.first, eit->first.second, eit->second))
			{
				result = false;
				break;
			}
		}
	}
	else
	{
		result = false;
	}
	return result;
}

bool AnimationSaver::savePoint(long int id, int x, int y, Frame::Diff::status status)
{
	bool res;
	res = fm.writeToFile<long int>(id);
	if (!res) return res;
	res = fm.writeToFile<int>(x);
	if (!res) return res;
	res = fm.writeToFile<int>(y);
	if (!res) return res;
	res = fm.writeToFile<Frame::Diff::status>(status);
	if (!res) return res;
	return res;
}

bool AnimationSaver::saveEdge(long int a, long int b, Frame::Diff::status status)
{
	bool res;
	res = fm.writeToFile<long int>(a);
	if (!res) return res;
	res = fm.writeToFile<long int>(b);
	if (!res) return res;
	res = fm.writeToFile<Frame::Diff::status>(status);
	if (!res) return res;
	return res;
}