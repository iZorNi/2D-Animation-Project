#pragma once
#include "stdafx.h"
#include <fstream>
#include <iostream>

//works with binary files
class FileManager
{
	std::ifstream inputFile;
	std::ofstream outputFile;

public:
	FileManager();
	~FileManager();
	bool openFileForWriting(std::string path);
	bool openFileForReading(std::string path);
	void closeInputFile();
	void closeOutputFile();
	template<typename T>
	bool writeToFile(T data);
	template<typename T>
	bool readFromFile(T& data);
};


template<typename T>
bool FileManager::writeToFile(T data)
{
	if (outputFile.out)
		outputFile.write(reinterpret_cast<const char *>(&data), sizeof(data));
	return outputFile.out;
}

template<typename T>
bool FileManager::readFromFile(T& data)
{
	if (inputFile.in)
	{
		char* tmp = new char[sizeof(data)];
		inputFile.read(reinterpret_cast<char*>(&data), sizeof(data));
		if (!inputFile)
		{
			inputFile.close();
			return false;
		}
	}
	return inputFile.in;

}