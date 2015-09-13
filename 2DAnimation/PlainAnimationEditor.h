#pragma once
#include "AnimationManager.h"
#include <string>

class PlainAnimationEditor
{
	std::shared_ptr<AnimationManager> slave;
	bool saveAnimation(std::string path);
	bool loadAnimation(std::string path);
public:
	PlainAnimationEditor();
	~PlainAnimationEditor();
	void runEditor(int* argcp, char **argv);
};

