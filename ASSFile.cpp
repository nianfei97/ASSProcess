#include "ASSFile.h"
#include <string>
#include <vector>
#include <fstream>

#define PREPROCESS_STOP_READ_LINE "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text"

ASSFile::ASSFile(std::string inFileName)
{
	std::ifstream fin(inFileName);

	ASSFile::readPreprocessLines(fin);
	ASSFile::processLyricLines(fin);

	fin.close();
}

void ASSFile::readPreprocessLines(std::ifstream &fin)
{
	std::string currLine;

	while (1)
	{
		getline(fin, currLine);
		this->preprocess.push_back(currLine);

		if (currLine == PREPROCESS_STOP_READ_LINE) break;
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