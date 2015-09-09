#pragma once
#include "FileManager.h"
#include "ISaver.h"

class FrameSaver: public ISaver
{
	std::string path;
	bool saveFrame(std::shared_ptr<Frame> frame);
	bool saveFrameID(unsigned int id);
	bool savePoints(std::shared_ptr<Frame> frame);
	bool saveEdges(std::shared_ptr<Frame> frame);
	bool savePoint(long int id, int x, int y, Frame::Diff::status status);
	bool saveEdge(long int a, long int b, Frame::Diff::status status);
	bool saveDifference(std::unique_ptr<Frame::Diff>& diff);
	FileManager fm;
public:
	FrameSaver();
	FrameSaver(std::string path);
	virtual ~FrameSaver();
	virtual bool saveContainer(IFrameContainer& container);
	virtual void setPath(char* path);
};

