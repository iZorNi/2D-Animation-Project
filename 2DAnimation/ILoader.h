#pragma once
#include "IFrameContainer.h"

class ILoader
{
public:
	virtual ~ILoader() {};
	virtual bool loadContainer(IFrameContainer & container) = 0;
	virtual void setPath(char* path) = 0;
};