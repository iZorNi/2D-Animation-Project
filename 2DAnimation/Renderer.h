#pragma once
#include "Frame.h"
#include "Dependencies\glew\GL\glew.h"
#include "Dependencies\freeglut\GL\freeglut.h"
#include <GL/gl.h>
#include <GL/glu.h>


class Renderer
{
	const unsigned int START_WIDTH = 800;
	const unsigned int START_HEIGHT = 600;
	const unsigned int POINT_SIZE = 6;
	const unsigned int LINE_WIDTH = 4;
	const char* name = "2D Animation Editor";
	int TEXT_POS_X;
	int TEXT_POS_Y;
	void* font = GLUT_BITMAP_HELVETICA_18;
	unsigned int window_width;
	unsigned int window_height;
	std::shared_ptr<Frame> currentFrame;
	void placePoint(int x,int y);
	void renderEdge(std::pair<int, int> A, std::pair<int, int> B);
	bool renderBackground();
	bool renderPoints();
	bool renderEdges();
	bool checkError();
	void checkSize(int& w, int& h);
	bool renderFrameNumber();
	static Renderer* wthis;

	static void sRenderFrame();

public:
	Renderer();
	~Renderer();
	Renderer(unsigned int width, unsigned int height);
	void renderFrame();
	void setCurrentFrame(std::shared_ptr<Frame> frame);
	bool restoreWindowSize(int width,int height);
	bool init(int* argcp, char **argv, std::shared_ptr<Frame> frame, int width, int height);
	void setWindowSize(int width, int height);
};

