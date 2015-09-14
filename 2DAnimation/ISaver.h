#pragma once
#include <memory>
#include "IFrameConroller.h"

class ISaver
{
public:
	ISaver() {};
	virtual ~ISaver() {}
	virtual bool saveAnimation(std::string path, std::weak_ptr<IFrameController> container) = 0;
};