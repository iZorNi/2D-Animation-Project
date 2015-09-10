#pragma once
#include "Frame.h"
#include "IUserInterfaceManager.h"
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
	int TEXT_RENDER_POSITION_X;
	int TEXT_RENDER_POSITION_Y;
	void* font = GLUT_BITMAP_HELVETICA_18;
	unsigned int window_width;
	unsigned int window_height;

	std::weak_ptr<IUserInterfaceManager> manager;
	void placePoint(int x,int y);
	void renderEdge(std::pair<int, int> A, std::pair<int, int> B);
	bool renderBackground();
	bool renderPoints(std::shared_ptr<Frame> renderedFrame);
	bool renderEdges(std::shared_ptr<Frame> renderedFrame);
	bool checkError();
	void checkSize(int& w, int& h);
	bool renderFrameNumber(std::shared_ptr<Frame> renderedFrame);

public:
	Renderer();
	void setSelfPointer(std::weak_ptr<Renderer> weakPtr);
	~Renderer();
	Renderer(unsigned int width, unsigned int height);
	void renderFrame();
	void renderFrame(Frame::PointIterator point, Frame::EdgeIterator edge);
	bool renderPoints(Frame::PointIterator point);
	bool renderEdges(Frame::EdgeIterator edge);
	bool restoreWindowSize(int width,int height);
	bool init(int* argcp, char **argv, int width, int height, std::weak_ptr<IUserInterfaceManager> uiManager);
	void setWindowSize(int width, int height);
};

