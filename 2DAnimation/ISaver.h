#pragma once
#include <memory>
#include "IFrameConroller.h"

class ISaver
{
public:
	ISaver() {};
	virtual ~ISaver() {}
	virtual bool saveAnimation(std::weak_ptr<IFrameController> container) = 0;
	virtual void setPath(std::string path) = 0;
};