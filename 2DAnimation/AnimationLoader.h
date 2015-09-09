#pragma once
#include "ILoader.h"
#include "FileManager.h"
class AnimationLoader: public ILoader
{
	std::string path;
	FileManager fm;

	typedef std::pair<long int, std::pair<std::shared_ptr<Point>, Frame::Diff::status>> dPoint;
	typedef std::pair< std::pair<long int, long int>, Frame::Diff::status> dEdge;
	std::shared_ptr<Frame> readFrame(std::shared_ptr<Frame> frame);
	//std::shared_ptr<Frame::Diff> readDiff();
	//void applyDiff(std::shared_ptr<Frame::Diff>);
	dPoint readPoint();
	bool loadPoints(std::shared_ptr<Frame> frame);
	bool loadPoint(std::shared_ptr<Frame> frame, dPoint point);
	dEdge readEdge();
	bool loadEdges(std::shared_ptr<Frame> frame);
	bool loadEdge(std::shared_ptr<Frame> frame, dEdge edge);

public:
	AnimationLoader();
	virtual ~AnimationLoader();
	virtual bool loadAnimation(std::weak_ptr<IFrameController> container);
	virtual void setPath(std::string path);
};

