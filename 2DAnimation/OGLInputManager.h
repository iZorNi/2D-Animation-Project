#pragma once
#include "IOGLManager.h"
#include "Dependencies\freeglut\GL\freeglut.h"
#include <memory>


enum MenuEntries
{
	ADD_POINT, REMOVE_POINT, REMOVE_EDGE
};

class OGLInputManager
{
	IOGLManager* mainManager;
	int menu;

public:

	OGLInputManager();
	~OGLInputManager();
	void handleKeys(unsigned char key, int x, int y);
	void handleMouse(int button, int state, int x, int y);
	void handleMouseMotion(int x, int y);
	void reshapeHandler(int width, int height);
	void closeFunc();
	void menuHandler(int val);
	void menuStatusHandler(int status, int x, int y);
	void createMenu();
	void init(IOGLManager* manager);
	void stop();
};

