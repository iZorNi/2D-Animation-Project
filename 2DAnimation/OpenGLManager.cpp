#include "OpenGLManager.h"


OpenGLManager::OpenGLManager()
{
	renderer = std::make_shared<Renderer>();
	input = std::make_shared<OpenGLInputManager>();
	renderer->setSelfPointer(std::weak_ptr<Renderer>(renderer));
	input->setSelfPointer(std::weak_ptr<OpenGLInputManager>(input));
}

void OpenGLManager::setSelfPointer(std::weak_ptr<OpenGLManager> weakPtr)
{
	weakPtrToThis = weakPtr;
}

void OpenGLManager::handleKeys(unsigned char key, int x, int y)
{
	if (key == PREVIOUS_FRAME_KEY_LOWER || key == PREVIOUS_FRAME_KEY_UPPER)
	{
		currentFrame = container.getPreviousFrame();
		renderer->setCurrentFrame(currentFrame);
	}
	else if (key == NEXT_FRAME_KEY_LOWER || key == NEXT_FRAME_KEY_UPPER)
	{
		currentFrame = container.getNextFrame();
		renderer->setCurrentFrame(currentFrame);
	}
	renderer->renderFrame();
}

void OpenGLManager::handleMouse(int key, int state, int x, int y)
{

	if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		currentFrame.lock()->tryPickPoint(x / POINT_SIZE, y / POINT_SIZE);
		leftPressed = true;
	}
	if (key == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		leftPressed = false;
	}
	render();
}

void OpenGLManager::handleMouseMotion(int x, int y)
{
	if (leftPressed)
	{
		currentFrame.lock()->movePoint(x / POINT_SIZE, y / POINT_SIZE);
		renderer->renderFrame();
	}
}

void OpenGLManager::reshapeHandler(int width, int height)
{
	renderer->restoreWindowSize(width, height);
}

void OpenGLManager::render()
{
	renderer->renderFrame();
}

void OpenGLManager::init(int width, int height, int* argcp, char **argv/*, std::weak_ptr<IManager> manager*/)
{
	if (!weakPtrToThis.expired())
	{
		leftPressed = false;
		this->manager = manager;
		currentFrame = container.getCurrentFrame();
		renderer->init(argcp, argv, width, height, weakPtrToThis);
		renderer->setCurrentFrame(currentFrame);
		input->init(weakPtrToThis);
	}
}

void OpenGLManager::run()
{
	//glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, 0);
	render();
	glutMainLoop();
}

void OpenGLManager::closeFunc()
{
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	input->stop();
	glutLeaveMainLoop();
}

void OpenGLManager::menuHandler(int val)
{
	switch (val) {
	case ADD_POINT:
		currentFrame.lock()->addPoint(menuPositionX / POINT_SIZE, menuPositionY / POINT_SIZE);
		break;
	case REMOVE_POINT:
		currentFrame.lock()->removePoint(menuPositionX / POINT_SIZE, menuPositionY / POINT_SIZE);
		break;
	case REMOVE_EDGE:
		currentFrame.lock()->removeEdge(menuPositionX / POINT_SIZE, menuPositionY / POINT_SIZE);
		break;
	}
	render();
}

void OpenGLManager::menuStatusHandler(int status, int x, int y)
{
	if (status == GLUT_MENU_IN_USE)
	{
		OpenGLManager::menuPositionX = x;
		OpenGLManager::menuPositionY = y;
	}
}

OpenGLManager::~OpenGLManager()
{
	currentFrame.reset();
	container.clear();
	renderer.reset();
	input.reset();
}