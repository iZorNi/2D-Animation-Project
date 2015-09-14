#pragma once
#include "FrameController.h"
#include "AnimationLoader.h"
#include "AnimationSaver.h"
#include "OpenGLManager.h"
#include "MenuManager.h"

class AnimationManager:public IManager
{
	std::shared_ptr<FrameController> container;
	std::shared_ptr<AnimationLoader> loader;
	std::shared_ptr<AnimationSaver> saver;
	std::shared_ptr<OpenGLManager> uiManager;
	std::shared_ptr<MenuManager> menu;
	std::weak_ptr<AnimationManager> weakPtrToThis;
public:
	AnimationManager();
	virtual ~AnimationManager();
	virtual bool save(std::string path);
	virtual bool load(std::string path);
	void init(int width, int height, int* argcp, char **argv);
	void run();
	virtual std::weak_ptr<Frame> getCurrentFrame();
	virtual void menuCall(int val, int x, int y) ;
	virtual void removePoint(int x, int y) ;
	virtual void addPoint(int x, int y) ;
	virtual void removeEdge(int x, int y) ;
	virtual void movePoint(int x, int y) ;
	virtual void pickPoint(int x, int y) ;
	virtual void freePickedPoint();
	virtual void nextFrame() ;
	virtual void previousFrame();
	void setWeakPointerToThis(std::weak_ptr<AnimationManager> ptr);
};
