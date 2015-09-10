#include "AnimationManager.h"

AnimationManager::AnimationManager()
{
	container = std::make_shared<FrameController>();
	saver = std::make_shared<AnimationSaver>();
	loader = std::make_shared<AnimationLoader>();
	menu = std::make_shared<MenuManager>();
	uiManager = std::make_shared<OpenGLManager>();
	uiManager->setSelfPointer(std::weak_ptr<OpenGLManager>(uiManager));
}

AnimationManager::~AnimationManager()
{
	container.reset();
	saver.reset();
	loader.reset();
	menu.reset();
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
void AnimationManager::menuCall(int val, int x, int y)
{
	menu->handleMenuCall(val, x, y);
}
void AnimationManager::removePoint(int x, int y)
{
	container->getCurrentFrame().lock()->removePoint(x, y);
}
void AnimationManager::addPoint(int x, int y)
{
	container->getCurrentFrame().lock()->addPoint(x, y);
}
void AnimationManager::removeEdge(int x, int y)
{
	container->getCurrentFrame().lock()->removeEdge(x, y);
}
void AnimationManager::movePoint(int x, int y)
{
	container->getCurrentFrame().lock()->movePoint(x, y);
}
void AnimationManager::pickPoint(int x, int y)
{
	container->getCurrentFrame().lock()->tryPickPoint(x,y);
}
void AnimationManager::nextFrame()
{
	container->getNextFrame();
}
void AnimationManager::previousFrame()
{
	container->getPreviousFrame();
}
std::weak_ptr<Frame> AnimationManager::getCurrentFrame()
{
	return container->getCurrentFrame();
}
void AnimationManager::init(int width, int height, int* argcp, char **argv)
{
	container->getFirstFrame();
	uiManager->init(width, height, argcp, argv, weakPtrToThis);
	menu->setMainManager(weakPtrToThis);
}
void AnimationManager::run()
{
	uiManager->run();
}
void AnimationManager::setWeakPointerToThis(std::weak_ptr<AnimationManager> ptr)
{
	weakPtrToThis = ptr;
}