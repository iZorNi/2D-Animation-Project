#pragma once
#include "Renderer.h"
#include "OpenGLInputManager.h"
#include "FrameController.h"
#include "IUserInterfaceManager.h"
#include "IManager.h"


class OpenGLManager:public IUserInterfaceManager
{
	//constants
	static const unsigned int POINT_SIZE = 6;
	static const unsigned int LINE_WIDTH = 4;
	static const unsigned char PREVIOUS_FRAME_KEY_LOWER = 'a';
	static const unsigned char NEXT_FRAME_KEY_LOWER = 'd';
	static const unsigned char PREVIOUS_FRAME_KEY_UPPER = 'A';
	static const unsigned char NEXT_FRAME_KEY_UPPER = 'D';
	static const unsigned char SCREEN_FPS = 60;

	std::weak_ptr<OpenGLManager> weakPtrToThis;
	int menuPositionX, menuPositionY;
	FrameController container;
	std::weak_ptr<Frame> currentFrame;
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<OpenGLInputManager> input;
	std::weak_ptr<IManager> manager;
	//private methods

public:
	OpenGLManager();
	void setSelfPointer(std::weak_ptr<OpenGLManager> weakPtr);
	virtual ~OpenGLManager();
	void init(int width, int height, int* argcp, char **argv/*, std::weak_ptr<IManager> manager*/);
	void run();
	bool leftPressed;
	virtual void handleKeys(unsigned char key, int x, int y);
	virtual void handleMouse(int button, int state, int x, int y);
	virtual void handleMouseMotion(int x, int y);
	virtual void reshapeHandler(int width, int height);
	void render();
	//static void runMainLoop(int val);
	virtual void closeFunc();
	virtual void menuHandler(int val);
	//void createMenu();
	virtual void menuStatusHandler(int status, int x, int y);
};

