#pragma once
#include "IManager.h"


//class handles menu options
//now there's only three, but if number grow it can be mainatained more easily
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

