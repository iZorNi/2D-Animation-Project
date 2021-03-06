#include "OpenGLInputManager.h"

namespace
{
	//pointer to this object, yes it's not safe, 
	//but still need it to interact with OpenGL calllbacks
	static std::weak_ptr<OpenGLInputManager> weakPtrToThis;
	//static calls
	//need this because opengl asks for (void*) functions and do not accept (Class::void*) functions
	static void sHandleKeys(unsigned char key, int x, int y)
	{
		if (!weakPtrToThis.expired())
		{
			weakPtrToThis.lock()->handleKeys(key, x, y);
		}
	}
	static void sHandleMouse(int button, int state, int x, int y)
	{
		if (!weakPtrToThis.expired())
		{
			weakPtrToThis.lock()->handleMouse(button, state, x, y);
		}
	}
	static void sHandleMouseMotion(int x, int y)
	{
		if (!weakPtrToThis.expired())
		{
			weakPtrToThis.lock()->handleMouseMotion(x, y);
		}
	}
	static void sReshapeHandler(int width, int height)
	{
		if (!weakPtrToThis.expired())
		{
			weakPtrToThis.lock()->reshapeHandler(width, height);
		}
	}
	static void sCloseFunc()
	{
		if (!weakPtrToThis.expired())
		{
			weakPtrToThis.lock()->closeFunc();
		}
	}
	static void sMenuHandler(int val)
	{
		if (!weakPtrToThis.expired())
		{
			weakPtrToThis.lock()->menuHandler(val);
		}
	}
	static void sMenuStatusHandler(int status, int x, int y)
	{
		if (!weakPtrToThis.expired())
		{
			weakPtrToThis.lock()->menuStatusHandler(status, x, y);
		}
	}
}
//sets weakPtrToThis
//void OpenGLInputManager::setSelfPointer(std::weak_ptr<OpenGLInputManager> weakPtr)
//{
//	weakPtrToThis = weakPtr;
//}

std::weak_ptr<OpenGLInputManager> OpenGLInputManager::instance;

std::shared_ptr<OpenGLInputManager> OpenGLInputManager::getInstance()
{
	if (instance.expired())
	{
		OpenGLInputManager tmp;
		std::shared_ptr<OpenGLInputManager> result = std::make_shared<OpenGLInputManager>(tmp);
		OpenGLInputManager::instance = std::weak_ptr<OpenGLInputManager>(result);
		weakPtrToThis = OpenGLInputManager::instance;
		return result;
	}
	else
	{
		return instance.lock();
	}
}

OpenGLInputManager::OpenGLInputManager()
{}

void OpenGLInputManager::handleKeys(unsigned char key, int x, int y)
{
	if (!mainManager.expired())
	{
		mainManager.lock()->handleKeys(key, x, y);
	}
}
void OpenGLInputManager::handleMouse(int button, int state, int x, int y)
{
	{
		if (!mainManager.expired())
		{
			mainManager.lock()->handleMouse(button,state, x, y);
		}
	}
}
void OpenGLInputManager::handleMouseMotion(int x, int y)
{
	if (!mainManager.expired())
	{
		mainManager.lock()->handleMouseMotion(x, y);
	}
}
void OpenGLInputManager::reshapeHandler(int width, int height)
{
	if (!mainManager.expired())
	{
		mainManager.lock()->reshapeHandler(width, height);
	}
}
void OpenGLInputManager::closeFunc()
{
	if (!mainManager.expired())
	{
		mainManager.lock()->closeFunc();
	}
}
void OpenGLInputManager::menuHandler(int val)
{
	if (!mainManager.expired())
	{
		mainManager.lock()->menuHandler(val);
	}
}
void OpenGLInputManager::menuStatusHandler(int status, int x, int y)
{
	if (!mainManager.expired())
	{
		mainManager.lock()->menuStatusHandler(status,x,y);
	}
}
void OpenGLInputManager::createMenu()
{
	menu = glutCreateMenu(sMenuHandler);

	glutAddMenuEntry("Add point", ADD_POINT);
	glutAddMenuEntry("Remove point", REMOVE_POINT);
	glutAddMenuEntry("Remove edge", REMOVE_EDGE);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void OpenGLInputManager::init(std::weak_ptr<IUserInterfaceManager> manager)
{
	if (!weakPtrToThis.expired())
	{
		mainManager = manager;


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
}
void OpenGLInputManager::stop()
{
	if (menu != 0)
	{
		//glutDestroyMenu(menu);
		glutKeyboardFunc(NULL);
		glutMotionFunc(NULL);
		glutMouseFunc(NULL);
		glutCloseFunc(NULL);
		glutReshapeFunc(NULL);
		glutDetachMenu(GLUT_RIGHT_BUTTON);
		glutMenuStatusFunc(NULL);
	}
}
