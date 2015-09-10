#pragma once
#include "FileManager.h"
#include "ISaver.h"

class AnimationSaver: public ISaver
{
	std::string path;
	bool saveFrame(std::shared_ptr<Frame> frame, FileManager& writer);
	bool saveFrameID(unsigned int id, FileManager& writer);
	bool savePoints(std::shared_ptr<Frame> frame, FileManager& writer);
	bool saveEdges(std::shared_ptr<Frame> frame, FileManager& writer);
	bool savePoint(long int id, int x, int y, Frame::Diff::status status, FileManager& writer);
	bool saveEdge(long int a, long int b, Frame::Diff::status status, FileManager& writer);
	bool saveDifference(std::unique_ptr<Frame::Diff>& diff, FileManager& writer);
	bool saveDifferencePoints(std::unique_ptr<Frame::Diff>& diff, FileManager& writer);
	bool saveDifferenceEdges(std::unique_ptr<Frame::Diff>& diff, FileManager& writer);
public:
	AnimationSaver();
	AnimationSaver(std::string path);
	virtual ~AnimationSaver();
	virtual bool saveAnimation(std::weak_ptr<IFrameController> container);
	virtual void setPath(std::string path);
};

