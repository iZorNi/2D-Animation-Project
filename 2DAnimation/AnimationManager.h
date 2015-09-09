#pragma once
#include "FrameContainer.h"
#include "FrameLoader.h"
#include "FrameSaver.h"
#include "OGLManager.h"

class AnimationManager
{
	FrameContainer container;
	FrameLoader loader;
	FrameSaver saver;

public:
	AnimationManager();
	~AnimationManager();
};

