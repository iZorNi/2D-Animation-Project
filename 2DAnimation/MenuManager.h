#pragma once
#include "IManager.h"



class MenuManager
{
	enum MenuEntries
	{
		ADD_POINT=0, REMOVE_POINT, REMOVE_EDGE
	};
	std::weak_ptr<IManager> manager;
public:
	MenuManager();
	~MenuManager();
	void setMainManager(std::weak_ptr<IManager> manager);
	void handleMenuCall(int val, int x, int y);
};

