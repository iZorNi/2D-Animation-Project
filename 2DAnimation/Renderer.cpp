#include "Renderer.h"


Renderer* Renderer::wthis;

void Renderer::renderFrame()
{
	if (currentFrame != nullptr)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderBackground();
		renderPoints();
		renderEdges();
		renderFrameNumber();
		glutSwapBuffers();
	}
}

void Renderer::sRenderFrame()
{
	if(wthis != nullptr)
	{
		wthis->renderFrame();
	}
}

void Renderer::setCurrentFrame(std::shared_ptr<Frame> frame)
{
	currentFrame = frame;
}

bool Renderer::renderPoints()
{
	long pickedPoint = currentFrame->getPickedPointId();
	glPointSize(POINT_SIZE);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	std::pair<long, std::shared_ptr<Point>> point = currentFrame->resetCurrentPoint();
	while (point != currentFrame->failurePoint)
	{
		if (point.first == pickedPoint)
		{
			glColor3f(.0f, 1.0f, 0.0f);
			placePoint(point.second->getX(), point.second->getY());
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else
		{
			placePoint(point.second->getX(), point.second->getY());
		}
		point = currentFrame->getNextPoint();
	}
	glEnd();
	GLenum error = glGetError();
	return checkError();
}

void Renderer::placePoint(int x, int y)
{
	x = x*POINT_SIZE + POINT_SIZE / 2;
	y = y*POINT_SIZE + POINT_SIZE / 2;
	glVertex2f(x, y);
}

bool Renderer::renderEdges()
{
	std::shared_ptr<Point> A, B;
	glLineWidth(LINE_WIDTH);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	std::pair<long, long> e = currentFrame->resetCurrentEdge();
	while (e.first != -1 && e.second != -1)
	{
		std::pair<long, std::shared_ptr<Point>> tmp1 = currentFrame->getPoint(e.first);
		std::pair<long, std::shared_ptr<Point>> tmp2 = currentFrame->getPoint(e.second);
		if (tmp1 != currentFrame->failurePoint && tmp2 != currentFrame->failurePoint)
		{
			A = tmp1.second;
			B = tmp2.second;
			renderEdge(std::make_pair(A->getX(), A->getY()), std::make_pair(B->getX(), B->getY()));
		}
		e = currentFrame->getNextEdge();
	}
	glEnd();
	return checkError();
}

void Renderer::renderEdge(std::pair<int,int> A, std::pair<int, int> B)
{
	placePoint(A.first, A.second);
	placePoint(B.first, B.second);
}

bool Renderer::renderBackground()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	return checkError();
}

bool Renderer::init(int* argcp, char **argv, std::shared_ptr<Frame> frame, int width, int height)
{
	wthis = this;
	glutInit(argcp, argv);
	currentFrame = frame;
	glutInitContextVersion(2, 1);

	//Create Double Buffered Window
	glutInitDisplayMode(GLUT_DOUBLE);
	setWindowSize(width, height);
	glutInitWindowSize(window_width, window_height);
	TEXT_POS_X = window_width - glutBitmapWidth(font, 'W');
	TEXT_POS_Y = glutBitmapHeight(font);
	glutCreateWindow(name);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, window_width, window_height, 0.0, 1.0, -1.0);

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(Renderer::sRenderFrame);

	//Check for error
	return checkError();
}

Renderer::Renderer() :window_width(START_WIDTH), window_height(START_HEIGHT)
{
	currentFrame = nullptr;
}

Renderer::Renderer(unsigned int width, unsigned int height)
{
	window_width = width;
	window_height = height;
	currentFrame = nullptr;
}

bool Renderer::restoreWindowSize(int width, int height)
{
	glutReshapeWindow(window_width, window_height);
	return checkError();
}

bool Renderer::checkError()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		return false;
	}

	return true;
}

void Renderer::setWindowSize(int width, int height)
{
	checkSize(width, height);
	window_width = width;
	window_height = height;
}

void Renderer::checkSize(int& w, int& h)
{
	if (w == 0)
	{
		w = START_WIDTH;
	}
	if (h == 0)
	{
		h = START_HEIGHT;
	}
	w = w < 0 ? 0 : w;
	w = w > glutGet(GLUT_SCREEN_WIDTH) ? glutGet(GLUT_SCREEN_WIDTH) : w;
	h = h < 0 ? 0 : h;
	h = h > glutGet(GLUT_SCREEN_HEIGHT) ? glutGet(GLUT_SCREEN_HEIGHT) : h;
}

bool Renderer::renderFrameNumber()
{
	const int size = 25;
	char s[size];
	sprintf_s(s, sizeof(s), "%d", currentFrame->getID()+1);
	unsigned char str[size];
	for (int i = 0; i < sizeof(s);++i)
	{
		str[i] = static_cast<unsigned char>(s[i]);
	}
	int offsetX = glutBitmapLength(font, str);
	glRasterPos2d(TEXT_POS_X-offsetX, TEXT_POS_Y);
	glColor3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; s[i] !='\0';++i)
	{
		glutBitmapCharacter(font, s[i]);
	}
	return checkError();
}

Renderer::~Renderer()
{
	currentFrame = nullptr;
}