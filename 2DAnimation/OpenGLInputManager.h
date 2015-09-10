#pragma once
#include "IUserInterfaceManager.h"
#include "Dependencies\freeglut\GL\freeglut.h"
#include <memory>


enum MenuEntries
{
	ADD_POINT=0, REMOVE_POINT, REMOVE_EDGE
};

class OpenGLInputManager
{
	std::weak_ptr<IUserInterfaceManager> mainManager;
	int menu;

public:
	void setSelfPointer(std::weak_ptr<OpenGLInputManager> weakPtr);
	void handleKeys(unsigned char key, int x, int y);
	void handleMouse(int button, int state, int x, int y);
	void handleMouseMotion(int x, int y);
	void reshapeHandler(int width, int height);
	void closeFunc();
	void menuHandler(int val);
	void menuStatusHandler(int status, int x, int y);
	void createMenu();
	void init(std::weak_ptr<IUserInterfaceManager> manager);
	void stop();
};

