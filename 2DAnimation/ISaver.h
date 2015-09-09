#pragma once
#include <memory>
#include "IFrameContainer.h"

class ISaver
{
public:
	ISaver() {};
	virtual ~ISaver() {}
	virtual bool saveContainer(IFrameContainer& container) = 0;
	virtual void setPath(char* path) = 0;
};