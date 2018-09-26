#include "ENLyricFile.h"

#include <fstream>

ENLyricFile::ENLyricFile(std::string inFile)
{
	std::ifstream fin(inFile);

	if (!fin.good())
		throw "Cannot find EN File!";

	std::string currLine;

	while (1)
	{
		getline(fin, currLine);
		if (currLine == "") break;

		this->metadata.push_back(currLine);
	}

	while (1)
	{
		getline(fin, currLine);

		if (fin.eof()) break;
		if (currLine == "") getline(fin, currLine);

		this->lines.push_back(currLine);
	}

	fin.close();
}

int ENLyricFile::getNumLines()
{
	return this->lines.size();
}

std::string ENLyricFile::getLine(int index)
{
	return this->lines[index];
}

std::string ENLyricFile::getTitle()
{
	return this->metadata[0];
}

std::vector <std::string> ENLyricFile::getMetadata()
{
	return this->metadata;
}