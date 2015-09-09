#pragma once
#include "FrameController.h"
#include "AnimationLoader.h"
#include "AnimationSaver.h"
#include "OpenGLManager.h"

class AnimationManager
{
	std::shared_ptr<FrameController> container;
	std::shared_ptr<AnimationLoader> loader;
	std::shared_ptr<AnimationSaver> saver;
	std::shared_ptr<OpenGLManager> uiManager;
public:
	AnimationManager();
	~AnimationManager();
	bool save(std::string path);
	bool load(std::string path);
	void init();
	void run();
};

