#pragma once
#include "Renderer.h"
#include "OGLInputManager.h"
#include "FrameContainer.h"
#include "IOGLManager.h"
#include "FrameLoader.h"
#include "FrameSaver.h"



class OGLManager:public IOGLManager
{
	//constants
	static const unsigned int POINT_SIZE = 6;
	static const unsigned int LINE_WIDTH = 4;
	static const unsigned char PREV = 'a';
	static const unsigned char NEXT = 'd';
	static const unsigned char PREV_UP = 'A';
	static const unsigned char NEXT_UP = 'D';
	static const unsigned char SCREEN_FPS = 60;

	int x, y, menu;
	FrameContainer fc;
	FrameLoader loader;
	FrameSaver saver;
	std::shared_ptr<Frame> currentFrame;
	Renderer rend;
	OGLInputManager input;
	bool canAddPoint, canRemovePoint, canRemoveEdge;
	//private methods

public:

	virtual ~OGLManager();
	void init(int width, int height, int* argcp, char **argv);
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
	bool save(char* path);
	bool load(char* path);
};

