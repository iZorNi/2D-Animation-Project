#include "MenuManager.h"

MenuManager::MenuManager()
{
}


MenuManager::~MenuManager()
{
}

void MenuManager::setMainManager(std::weak_ptr<IManager> manager)
{
	this->manager = manager;
}

void MenuManager::handleMenuCall(int val, int x, int y)
{
	switch (val)
	{
	case MenuEntries::ADD_POINT:
		manager.lock()->addPoint(x, y);
		break;
	case MenuEntries::REMOVE_POINT:
		manager.lock()->removePoint(x, y);
		break;
	case MenuEntries::REMOVE_EDGE:
		manager.lock()->removeEdge(x, y);
		break;
	default:
		break;
	}
}