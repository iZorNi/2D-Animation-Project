#pragma once
#include "IManager.h"

enum MenuEntries
{
	ADD_POINT, REMOVE_POINT, REMOVE_EDGE
};

class MenuManager
{
	std::weak_ptr<IManager> manager;
public:
	MenuManager();
	~MenuManager();
	void setMainManager(std::weak_ptr<IManager> manager);
	void handleMenuCall(int val, int x, int y);
};

