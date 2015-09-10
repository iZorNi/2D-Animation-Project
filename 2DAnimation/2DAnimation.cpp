// 2DAnimation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PlainAnimationEditor.h"
#include <iostream>
#include <string>

int main(int argc, char* args[])
{
	PlainAnimationEditor editor;
	editor.runEditor(&argc, args);
	return 0;
}

