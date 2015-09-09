#include "OGLManager.h"


void OGLManager::handleKeys(unsigned char key, int x, int y)
{
	if (key == PREV || key == PREV_UP)
	{
		currentFrame = fc.prev();
		rend.setCurrentFrame(currentFrame);
	}
	else if (key == NEXT || key == NEXT_UP)
	{
		currentFrame = fc.next();
		rend.setCurrentFrame(currentFrame);
	}
	rend.renderFrame();
}

void OGLManager::handleMouse(int key, int state, int x, int y)
{

	if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		currentFrame->tryPickPoint(x / POINT_SIZE, y / POINT_SIZE);
		leftPressed = true;
	}
	if (key == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		leftPressed = false;
	}
	render();
}

void OGLManager::handleMouseMotion(int x, int y)
{
	if (leftPressed)
	{
		currentFrame->movePoint(x / POINT_SIZE, y / POINT_SIZE);
		rend.renderFrame();
	}
}

void OGLManager::reshapeHandler(int width, int height)
{
	rend.restoreWindowSize(width, height);
}

void OGLManager::render()
{
	rend.renderFrame();
}

void OGLManager::init(int width, int height, int* argcp, char **argv)
{
	leftPressed = false;
	currentFrame = fc.getCurrent();
	rend.init(argcp,argv, currentFrame, width, height);
	input.init(this);
}

void OGLManager::run()
{
	//glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, 0);
	render();
	glutMainLoop();
}

//void OGLManager::runMainLoop(int val)
//{
//	if (val != -1)
//	{
//		update();
//		rend.renderFrame();
//
//		glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, val);
//	}
//}

void OGLManager::closeFunc()
{
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	input.stop();
	glutLeaveMainLoop();
}

void OGLManager::menuHandler(int val)
{
	//canRemoveEdge = currentFrame->tryPickEdge(x / POINT_SIZE, y / POINT_SIZE);
	//currentFrame->freePoint();
	//canRemovePoint = currentFrame->tryPickPoint(x / POINT_SIZE, y / POINT_SIZE);
	//canAddPoint = !canRemoveEdge && !canRemovePoint;
	switch (val) {
	case ADD_POINT:
		if (! (currentFrame->tryPickEdge(x / POINT_SIZE, y / POINT_SIZE)) &&
			! (currentFrame->tryPickPoint(x / POINT_SIZE, y / POINT_SIZE)) )
		{
			currentFrame->addPoint(x / POINT_SIZE, y / POINT_SIZE);
		}
		break;
	case REMOVE_POINT:
		if (canRemovePoint = currentFrame->tryPickPoint(x / POINT_SIZE, y / POINT_SIZE))
		{
			currentFrame->removePoint();
		}

		break;
	case REMOVE_EDGE:
		if (currentFrame->tryPickEdge(x / POINT_SIZE, y / POINT_SIZE))
		{
			currentFrame->removeEdge();
		}
		break;
	}
	currentFrame->freePoint();
	render();
}

void OGLManager::menuStatusHandler(int status, int x, int y)
{
	if (status == GLUT_MENU_IN_USE)
	{
		OGLManager::x = x;
		OGLManager::y = y;
		
	}
}

OGLManager::~OGLManager()
{
	currentFrame = nullptr;
	fc.clear();
}

bool  OGLManager::save(char* path)
{
	saver.setPath(path);
	return saver.saveContainer(fc);
}

bool  OGLManager::load(char* path)
{
	loader.setPath(path);
	return loader.loadContainer(fc);
}