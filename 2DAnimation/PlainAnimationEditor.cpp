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

std::string getPath()
{
	std::string path;
	std::cout << "\nEnter path:  ";
	std::cin >> path;
	std::cout << "\n";
	return path;
}

void PlainAnimationEditor::runEditor(int* argcp, char **argv)
{
	std::cout << "If you want to load file press any key, else press enter\n";
	std::string inputPath = "";
	char c = _getch();
	if(c != VK_RETURN)
	{
		inputPath = getPath();
	}
	if (inputPath != "")
	{
		if (!loadAnimation(inputPath))
		{
			std::cout << "Fail...    ";
		}
	}
	slave->init(0, 0, argcp, argv);
	slave->run();
	std::cout << "\nDo you want to save animation?(y/n)  ";
	c = _getch();
	while (c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		c = _getch();
	}
	std::cout << c << '\n';
	if (c == 'y' || c == 'Y')
	{
		std::cout << "Press any key or press enter to save to the input file\n  ";
		std::string outputPath;
		c = _getch();
		if (c != VK_RETURN)
		{
			outputPath = getPath();
		}
		else
		{
			if (outputPath == "" && inputPath != "")
			{
				outputPath = inputPath;
			}
		}
		if (outputPath != "")
		{
			if (saveAnimation(outputPath))
			{
				std::cout << "Ready...  ";
			}
			else
			{
				std::cout << "Fail...    ";
			}
		}
		else
		{
			std::cout << "Fail...    ";
		}
	}
	std::cout << "Press any key... ";
	c = _getche();
}