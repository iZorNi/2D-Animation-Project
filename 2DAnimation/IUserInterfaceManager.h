#pragma once
#include "Frame.h"

class IUserInterfaceManager
{
public:
	virtual ~IUserInterfaceManager() {}
	virtual void handleKeys(unsigned char key, int x, int y) = 0;
	virtual void handleMouse(int button, int state, int x, int y) = 0;
	virtual void handleMouseMotion(int x, int y)  = 0;
	virtual void reshapeHandler(int width, int height)  = 0;
	virtual void closeFunc()  = 0;
	virtual void menuHandler(int val)  = 0;
	virtual void menuStatusHandler(int status, int x, int y)  = 0;
	virtual std::weak_ptr<Frame> getCurrentFrame() = 0;
private:

};