#include "AnimationManager.h"



AnimationManager::AnimationManager()
{
	container = std::make_shared<FrameController>();
	saver = std::make_shared<AnimationSaver>();
	loader = std::make_shared<AnimationLoader>();
	uiManager = std::make_shared<OpenGLManager>();
	uiManager->setSelfPointer(std::weak_ptr<OpenGLManager>(uiManager));
}

AnimationManager::~AnimationManager()
{
	container.reset();
	saver.reset();
	loader.reset();
	uiManager.reset();
}

bool AnimationManager::save(std::string path)
{
	saver->setPath(path);
	return saver->saveAnimation(std::weak_ptr<IFrameController>(container));
}

bool AnimationManager::load(std::string path)
{
	loader->setPath(path);
	return loader->loadAnimation(std::weak_ptr<IFrameController>(container));
}