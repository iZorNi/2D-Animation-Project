#pragma once
#include "IFrameConroller.h"

class ILoader
{
public:
	virtual ~ILoader() {};
	virtual bool loadAnimation(std::string path, std::weak_ptr<IFrameController> container) = 0;
};