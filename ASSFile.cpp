#include "ASSFile.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#define PREPROCESS_STOP_READ_LINE "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text"

ASSFile::ASSFile()
{

}

ASSFile::ASSFile(std::string inFileName)
{
	std::ifstream fin(inFileName);

	this->title = convertUnderscoreToSpace(inFileName);

	readPreprocessLines(fin);
	processLyricLines(fin);

	fin.close();
}

std::string ASSFile::convertUnderscoreToSpace(std::string input)
{
	std::string result;

	for (char c : input)
	{
		if (c == '_') result += ' ';
		else result += c;
	}

	return result;
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
		getline(fin, currLineString);

		if (fin.eof()) break;

		ASSLine currLine(currLineString);

		this->lines.push_back(currLine);
	}
}

void ASSFile::printFile(std::string outFileName)
{
	std::ofstream fout(outFileName);

	for (std::string line : this->preprocess)
		fout << line << std::endl;

	for (ASSLine line : this->lines)
		fout << line.printASSLine();
}

int ASSFile::getNumLines()
{
	return lines.size();
}

ASSFileWithSwitch::ASSFileWithSwitch(std::string inFileName)
{
	std::ifstream fin(inFileName);

	try
	{
		readPreprocessLines(fin);
		processLyricLines(fin);
	}
	catch (const char* exception)
	{
		throw exception;
	}

	fin.close();
}

void ASSFileWithSwitch::processLyricLines(std::ifstream &fin)
{
	std::vector <ASSLine> currLines;

	while (1)
	{
		std::string currLineString;
		getline(fin, currLineString);

		if (fin.eof())
		{
			try
			{
				ASSLineWithSwitch currLinesWithSwitch(currLines);
				this->lines.push_back(currLinesWithSwitch);
			} 
			catch (const char* exception)
			{
				for (ASSLine errorLine : currLines)
				{
					std::cerr << errorLine.printASSLine();
				}
				throw exception;
			}
			
			break;
		}

		ASSLine currLine(currLineString);
		
		if (currLines.empty() || currLines.rbegin()->getText() == currLine.getText())
		{
			currLines.push_back(currLine);
		}
		else
		{
			try
			{
				ASSLineWithSwitch currLinesWithSwitch(currLines);
				this->lines.push_back(currLinesWithSwitch);
				currLines.clear();
				currLines.push_back(currLine);
			}
			catch (const char* exception)
			{
				for (ASSLine errorLine : currLines)
				{
					std::cerr << errorLine.printASSLine();
				}
				throw exception;
			}
		}
	}
}

void ASSFileWithSwitch::printFile(std::string outFileName)
{
	std::ofstream fout(outFileName);

	for (std::string line : this->preprocess)
		fout << line << std::endl;

	for (ASSLineWithSwitch line : this->lines)
		fout << line.printASSLine();
}

int ASSFileWithSwitch::getNumLines()
{
	return lines.size();
}

