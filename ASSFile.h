#ifndef __ASSFile__
#define __ASSFile__

#include "ASSLine.h"
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
	
	// Converts all underscores in a std::string to spaces
	std::string convertUnderscoreToSpace(std::string input);

	void readPreprocessLines(std::ifstream &fin);
	void processLyricLines(std::ifstream &fin);

public:
	ASSFile(std::string inFileName);
	void removeAllTags();
	void removeNonKaraokeTags();
	void printFile(std::string outFileName);
	int getNumLines();
};

class ASSFileWithSwitch : public ASSFile
{
private:
	void processLyricLines(std::ifstream &fin);
	std::vector <ASSLineWithSwitch> lines;

public:
	ASSFileWithSwitch(std::string inFileName);
	void printFile(std::string outFileName);
	int getNumLines();
};

#endif