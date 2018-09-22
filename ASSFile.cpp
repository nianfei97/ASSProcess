#include "ASSFile.h"

#include <string>
#include <vector>
#include <fstream>

ASSFile::ASSFile(std::string inFileName)
{
	std::ifstream fin(inFileName);

	ASSFile::skipPreprocessLines(fin);
	ASSFile::processLyricLines(fin);

	fin.close();
}

void ASSFile::skipPreprocessLines(std::ifstream &fin)
{
	std::string currLine;

	while (1)
	{
		getline(fin, currLine);

		if (currLine == "[Events]") 
		{
			getline(fin, currLine);
			break;
		}
	}
}

void ASSFile::removeAllTags()
{
	for (ASSLine line : this->lines)
		line.removeAllTags();
}

void ASSFile::removeNonKaraokeTags()
{
	for (ASSLine line : this->lines)
		line.removeNonKaraokeTags();
}

void ASSFile::processLyricLines(std::ifstream &fin)
{
	while (1)
	{
		std::string currLineString;
		fin >> currLineString;

		if (fin.eof()) break;

		ASSLine currLine(currLineString);

		if (currLine.getLayer() == 0) this->uniqueLines++;

		this->lines.push_back(currLine);
	}
}