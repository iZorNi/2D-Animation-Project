#include "PlainAnimationEditor.h"
#include <conio.h>

PlainAnimationEditor::PlainAnimationEditor()
{
	slave = std::make_shared<AnimationManager>();
	slave->setWeakPointerToThis(std::weak_ptr<AnimationManager>(slave));
}


PlainAnimationEditor::~PlainAnimationEditor()
{
}


bool PlainAnimationEditor::saveAnimation(std::string path)
{
	return slave->save(path);
}
bool PlainAnimationEditor::loadAnimation(std::string path)
{
	return slave->load(path);
}
void PlainAnimationEditor::runEditor(int* argcp, char **argv)
{
	std::cout << "If you want to load file then enter path, else press enter\n";
	std::string inputPath = "";
	char c = _getch();
	while (c == 0)
	{
		c = _getche();
		if (c == VK_RETURN)
			break;
	}
	if(c != VK_RETURN)
	{
		_ungetch(c);
		std::cin >> inputPath;
	}
	inputPath = "D:/animation.paf";
	if (inputPath != "")
	{
		loadAnimation(inputPath);
	}
	slave->init(0, 0, argcp, argv);
	slave->run();
	std::cout << "Do you want to save animation?(y/n)  ";
	c = _getche();
	while (c == 0)
	{
		c = _getche();
		c = _getche();
	}
	if (c == 'y' || c == 'Y')
	{
		std::cout << "\nEnter path or press enter to save to the input file\n  ";
		std::string outputPath;
		//c = _getche();
		//if (c == 0)
		//{
		//	c = _getche();
		//}
		//else
		//{
		//	_ungetch(c);
			outputPath = "D:/animation.paf";
			std::cin >> outputPath;
		//}
		if (outputPath == "")
		{
			outputPath = inputPath;
		}
		if (saveAnimation(outputPath))
		{
			std::cout << "Ready...  ";
		}
		else
		{
			std::cout << "Fail...    ";
		}
	}
	std::cout << "Press any key... ";
	c = _getche();
}