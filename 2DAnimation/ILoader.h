#pragma once
#include "IFrameConroller.h"

class ILoader
{
public:
	virtual ~ILoader() {};
	virtual bool loadAnimation(std::weak_ptr<IFrameController> container) = 0;
	virtual void setPath(std::string path) = 0;
};