// 2DAnimation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OGLManager.h"
#include <iostream>

int main(int argc, char* args[])
{
	int w, h;
	//std::cout << "Enter desirable window size(width,height): 0 0 to get default\n";
	//std::cin >> w >> h;
	OGLManager om;
	om.load("D:/animation.paf");
	om.init(0, 0, &argc, args);
	om.run();
	std::cout << "Do you want to save animation?(Y / N) \n";
	char c;
	std::cin >> c;
	if (c == 'Y' || c == 'y')
	{
		if (om.save("D:/animation.paf"))
		{
			std::cout << "Ready... Press any key\n";
			std::cin >> c;
		}
		else
		{
			std::cout << "Fail... Press any key\n";
			std::cin >> c;
		}
	}
    return 0;
}

