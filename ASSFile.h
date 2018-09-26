#ifndef __ASSFile__
#define __ASSFile__

#include "ASSLine.h"
#include "ASSLineWithSwitch.h"
#include <vector>
#include <string>
#include <fstream>

class ASSFile
{
private:
	std::vector <ASSLine> lines;

protected:
	std::vector <std::string> preprocess;
	std::string title;

	ASSFile();
	ASSFile(std::string inFileName);

	// Converts all underscores in a std::string to spaces
	std::string convertUnderscoreToSpace(std::string input);

	void readPreprocessLines(std::ifstream &fin);
	void processLyricLines(std::ifstream &fin);

public:
	void readPreprocessLines(std::string inFileName);

	void removeAllTags();
	void removeNonKaraokeTags();
	void printASSFile(std::string outFileName);

	int getNumLines();
	ASSTime getEndTime();

	void insertLine(ASSLine toInsert);
};

class ASSFileWithSwitch : public ASSFile
{
private:
	std::vector <ASSLineWithSwitch> lines;

	void processLyricLines(std::ifstream &fin);

public:
	ASSFileWithSwitch();
	ASSFileWithSwitch(std::string inFileName);

	void operator=(ASSFileWithSwitch toSet);

	std::vector <ASSLineWithSwitch> getLines();
	ASSLineWithSwitch getLine(int index);
	void clearLines();

	void printASSFile(std::string outFileName);
	
	int getNumLines();
	ASSTime getEndTime();

	void insertLine(ASSLineWithSwitch toInsert);
};

#endif