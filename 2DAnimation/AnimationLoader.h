#pragma once
#include "ILoader.h"
#include "FileManager.h"
class AnimationLoader: public ILoader
{
	typedef std::pair<long int, std::pair<std::shared_ptr<Point>, Frame::Diff::status>> dPoint;
	typedef std::pair< std::pair<long int, long int>, Frame::Diff::status> dEdge;
	std::shared_ptr<Frame> readFrame(std::shared_ptr<Frame> frame, FileManager& reader);
	//std::shared_ptr<Frame::Diff> readDiff();
	//void applyDiff(std::shared_ptr<Frame::Diff>);
	dPoint readPoint(FileManager& reader);
	bool loadPoints(std::shared_ptr<Frame> frame, FileManager& reader);
	bool loadPoint(std::shared_ptr<Frame> frame, dPoint point);
	dEdge readEdge(FileManager& reader);
	bool loadEdges(std::shared_ptr<Frame> frame, FileManager& reader);
	bool loadEdge(std::shared_ptr<Frame> frame, dEdge edge);

public:
	virtual ~AnimationLoader();
	virtual bool loadAnimation(std::string path, std::weak_ptr<IFrameController> container);
};

