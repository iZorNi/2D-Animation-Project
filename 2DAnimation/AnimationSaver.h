#pragma once
#include "FileManager.h"
#include "ISaver.h"
//saves animation in binary file
class AnimationSaver: public ISaver
{
	bool savePoint(long int id, int x, int y, Frame::Diff::status status, FileManager& writer);
	bool saveEdge(long int a, long int b, Frame::Diff::status status, FileManager& writer);
	//saves difference between frames
	bool saveDifference(std::unique_ptr<Frame::Diff>& diff, FileManager& writer);
	bool saveDifferencePoints(std::unique_ptr<Frame::Diff>& diff, FileManager& writer);
	bool saveDifferenceEdges(std::unique_ptr<Frame::Diff>& diff, FileManager& writer);
public:
	virtual ~AnimationSaver();
	virtual bool saveAnimation(std::string path, std::weak_ptr<IFrameController> container);
};

