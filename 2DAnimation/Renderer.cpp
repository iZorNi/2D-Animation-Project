#include "Renderer.h"

namespace
{
	//pointer to this object, yes it's not safe, 
	//but still need it to interact with OpenGL calllbacks
	static std::weak_ptr<Renderer> weakPtrToThis;
	//static calls
	//need this because opengl asks for (void*) functions and do not accept (Class::void*) functions
	static void sRenderFrame()
	{
		if (!weakPtrToThis.expired())
		{
			weakPtrToThis.lock()->renderFrame();
		}
	}
}

//void Renderer::setSelfPointer(std::weak_ptr<Renderer> weakPtr)
//{
//	::weakPtrToThis = weakPtr;
//}

std::weak_ptr<Renderer> Renderer::instance;

void Renderer::renderFrame()
{
	if (!manager.expired())
	{
		std::shared_ptr<Frame> renderedFrame = manager.lock()->getCurrentFrame().lock();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderBackground();
		renderPoints(renderedFrame->pointsBegin(), renderedFrame->getNumberOfPoints(), std::bind(static_cast<bool(Frame::*)(long int)>(&Frame::isPickedPoint), &(*renderedFrame), std::placeholders::_1));
		renderEdges(renderedFrame->edgesBegin(), renderedFrame->getNumberOfEdges(), renderedFrame->pointsEnd(), std::bind(&Frame::getPoint, &(*renderedFrame), std::placeholders::_1));
		renderFrameNumber(renderedFrame);
		glutSwapBuffers();
	}
}

std::shared_ptr<Renderer> Renderer::getInstance()
{
	if (Renderer::instance.expired())
	{
		Renderer tmp;
		std::shared_ptr<Renderer> result = std::make_shared<Renderer>(tmp);
		Renderer::instance = std::weak_ptr<Renderer>(result);
		weakPtrToThis = Renderer::instance;
		return result;
	}
	else
	{
		return instance.lock();
	}
}

bool Renderer::renderPoints(PointsContainer::PointIterator pointsBegin, int size, std::function<bool(long)> isPickedPoint)
{
	glPointSize(POINT_SIZE);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	auto pointIterator = pointsBegin;
	for (int i = 0; i < size; ++i, ++pointIterator)
	{
		if (isPickedPoint(pointIterator.getId()))
		{
			glColor3f(.0f, 1.0f, 0.0f);
			placePoint(pointIterator->getX(), pointIterator->getY());
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else
		{
			placePoint(pointIterator->getX(), pointIterator->getY());
		}
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

bool Renderer::renderEdges(EdgesContainer::EdgeIterator edgesBegin, int size, PointsContainer::iterator pointsEnd, std::function<PointsContainer::PointIterator(long)> getPoint)
{
	PointsContainer::PointIterator A, B;
	glLineWidth(LINE_WIDTH);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	auto edgeIterator = edgesBegin;
	for (int i = 0; i < size; ++i, ++edgeIterator)
	{
		A = getPoint(edgeIterator->first);
		B = getPoint(edgeIterator->second);
		if (A != pointsEnd && B != pointsEnd)
		{
			renderEdge(std::make_pair(A->getX(), A->getY()), std::make_pair(B->getX(), B->getY()));
		}
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

bool Renderer::init(int* argcp, char **argv, int width, int height, std::weak_ptr<IUserInterfaceManager> manager)
{
	this->manager = manager;
	glutInit(argcp, argv);
	glutInitContextVersion(2, 1);

	//Create Double Buffered Window
	glutInitDisplayMode(GLUT_DOUBLE);
	setWindowSize(width, height);
	glutInitWindowSize(window_width, window_height);
	TEXT_RENDER_POSITION_X = window_width - glutBitmapWidth(font, 'W');
	TEXT_RENDER_POSITION_Y = glutBitmapHeight(font);
	glutCreateWindow(name);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, window_width, window_height, 0.0, 1.0, -1.0);

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(sRenderFrame);

	//Check for error
	return checkError();
}

Renderer::Renderer()
	:window_width(START_WIDTH), window_height(START_HEIGHT)
{}

Renderer::Renderer(unsigned int width, unsigned int height):
	window_width(width), window_height(height)
{}

Renderer::Renderer(const Renderer& value)
{
	this->manager = value.manager;
	this->window_width = value.window_width;
	this->window_height = value.window_height;
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

bool Renderer::renderFrameNumber(std::shared_ptr<Frame> renderedFrame)
{
	const int size = 25;
	char s[size];
	sprintf_s(s, sizeof(s), "%d", renderedFrame->getID()+1);
	unsigned char str[size];
	for (int i = 0; i < sizeof(s);++i)
	{
		str[i] = static_cast<unsigned char>(s[i]);
	}
	int offsetX = glutBitmapLength(font, str);
	glRasterPos2d(TEXT_RENDER_POSITION_X-offsetX, TEXT_RENDER_POSITION_Y);
	glColor3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; s[i] !='\0';++i)
	{
		glutBitmapCharacter(font, s[i]);
	}
	return checkError();
}

Renderer::~Renderer()
{
}