#include "OGLInputManager.h"

static OGLInputManager* wthis;

static void sHandleKeys(unsigned char key, int x, int y)
{
	if (wthis != nullptr)
	{
		wthis->handleKeys(key, x, y);
	}
}
static void sHandleMouse(int button, int state, int x, int y)
{
	if (wthis != nullptr)
	{
		wthis->handleMouse(button, state, x, y);
	}
}
static void sHandleMouseMotion(int x, int y)
{
	if (wthis != nullptr)
	{
		wthis->handleMouseMotion(x, y);
	}
}
static void sReshapeHandler(int width, int height)
{
	if (wthis != nullptr)
	{
		wthis->reshapeHandler(width, height);
	}
}
static void sCloseFunc()
{
	if (wthis != nullptr)
	{
		wthis->closeFunc();
	}
}
static void sMenuHandler(int val)
{
	if (wthis != nullptr)
	{
		wthis->menuHandler(val);
	}
}
static void sMenuStatusHandler(int status, int x, int y)
{
	if (wthis != nullptr)
	{
		wthis->menuStatusHandler(status, x, y);
	}
}


OGLInputManager::OGLInputManager()
{

}

OGLInputManager::~OGLInputManager()
{
	wthis = nullptr;
}

void OGLInputManager::handleKeys(unsigned char key, int x, int y)
{
	if (mainManager!=nullptr)
	{
		mainManager->handleKeys(key, x, y);
	}
}
void OGLInputManager::handleMouse(int button, int state, int x, int y)
{
	{
		if (mainManager != nullptr)
		{
			mainManager->handleMouse(button,state, x, y);
		}
	}
}
void OGLInputManager::handleMouseMotion(int x, int y)
{
	if (mainManager != nullptr)
	{
		mainManager->handleMouseMotion(x, y);
	}
}
void OGLInputManager::reshapeHandler(int width, int height)
{
	if (mainManager != nullptr)
	{
		mainManager->reshapeHandler(width, height);
	}
}
void OGLInputManager::closeFunc()
{
	if (mainManager != nullptr)
	{
		mainManager->closeFunc();
	}
}
void OGLInputManager::menuHandler(int val)
{
	if (mainManager != nullptr)
	{
		mainManager->menuHandler(val);
	}
}
void OGLInputManager::menuStatusHandler(int status, int x, int y)
{
	if (mainManager != nullptr)
	{
		mainManager->menuStatusHandler(status,x,y);
	}
}
void OGLInputManager::createMenu()
{
	menu = glutCreateMenu(sMenuHandler);

	glutAddMenuEntry("Add point", ADD_POINT);
	glutAddMenuEntry("Remove point", REMOVE_POINT);
	glutAddMenuEntry("Remove edge", REMOVE_EDGE);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void  OGLInputManager::init(IOGLManager* manager)
{
	mainManager = manager;
	wthis = this;


	//Set keyboard handler
	glutKeyboardFunc(sHandleKeys);

	//Set mouse handler
	glutMouseFunc(sHandleMouse);

	//Set mouse motion handler
	glutMotionFunc(sHandleMouseMotion);

	//Set reshape preventer
	glutReshapeFunc(sReshapeHandler);

	//Set glut close function
	glutCloseFunc(sCloseFunc);

	glutMenuStatusFunc(sMenuStatusHandler);

	createMenu();
}

void  OGLInputManager::stop()
{
	glutDestroyMenu(menu);
	glutKeyboardFunc(NULL);
	glutMotionFunc(NULL);
	glutMouseFunc(NULL);
	glutCloseFunc(NULL);
	glutReshapeFunc(NULL);
	glutDetachMenu(GLUT_RIGHT_BUTTON);
	glutMenuStatusFunc(NULL);
	wthis = nullptr;
}
