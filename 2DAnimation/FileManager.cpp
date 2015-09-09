#include "stdafx.h"
#include "FileManager.h"

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
	closeInputFile();
	closeOutputFile();
}

bool FileManager::openFileForWriting(std::string path)
{
	if (outputFile.is_open())
	{
		outputFile.close();
	}
	outputFile.open(path, std::ofstream::binary | std::ofstream::trunc);
	return outputFile.is_open();
}

bool FileManager::openFileForReading(std::string path)
{
	if (inputFile.is_open())
	{
		inputFile.close();
	}
	inputFile.open(path, std::ios::binary);
	return inputFile.is_open();
}

void FileManager::closeInputFile()
{
	if (inputFile.is_open())
	{
		inputFile.close();
	}
}

void FileManager::closeOutputFile()
{
	if (outputFile.is_open())
	{
		outputFile.close();
	}
}

