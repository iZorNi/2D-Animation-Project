#include "FrameSaver.h"



FrameSaver::FrameSaver():path("")
{
}

FrameSaver::~FrameSaver()
{
}

FrameSaver::FrameSaver(std::string path) :path(path)
{
};

bool FrameSaver::saveContainer(IFrameContainer& container)
{
	bool res = path.length() != 0;
	if (res)
	{
		res = fm.openFileForWriting(path);
		if (res)
		{
			res = fm.writeToFile<int>(container.total());
			if (!res)
			{
				fm.closeOutputFile();
				return res;
			}
			std::shared_ptr<Frame> f = container.begin();
			res = saveFrame(f);
			if (!res)
			{
				fm.closeOutputFile();
				return res;
			}
			while(f!=container.last())
			{
				std::unique_ptr<Frame::Diff> diff =  container.next()->getDiff(*f);
				res = saveDifference(diff);
				if (!res)
				{
					fm.closeOutputFile();
					return res;
				}
				f = container.getCurrent();
			}
		}

	}
	return res;
}

void FrameSaver::setPath(char* path)
{
	this->path = path;
}

bool FrameSaver::saveFrame(std::shared_ptr<Frame> frame)
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

bool FrameSaver::saveFrameID(unsigned int id)
{
	return fm.writeToFile<unsigned int>(id);
}

bool FrameSaver::savePoints(std::shared_ptr<Frame> frame)
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

bool FrameSaver::saveEdges(std::shared_ptr<Frame> frame)
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

bool FrameSaver::saveDifference(std::unique_ptr<Frame::Diff>& diff)
{
	bool res;
	res = fm.writeToFile<unsigned int>(diff->id);
	if (!res) return res;
	//points
	res = fm.writeToFile<unsigned int>(diff->points.size());
	if (!res) return res;
	std::map<long int, std::pair<std::shared_ptr<Point>,Frame::Diff::status>>::iterator pit = diff->points.begin();
	while (pit != diff->points.end())
	{
		res = savePoint(pit->first, pit->second.first->getX(), pit->second.first->getY(),pit->second.second);
		if (!res) return res;
	}
	//edges
	res = fm.writeToFile<unsigned int>(diff->edges.size());
	if (!res) return res;
	std::set<std::pair<std::pair<long int, long int>,Frame::Diff::status>>::iterator eit = diff->edges.begin();
	while (eit != diff->edges.end())
	{
		res = saveEdge(eit->first.first, eit->first.second, eit->second);
		if (!res) return res;
	}
	return res;
}

bool FrameSaver::savePoint(long int id, int x, int y, Frame::Diff::status status)
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

bool FrameSaver::saveEdge(long int a, long int b, Frame::Diff::status status)
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