#ifndef __ASSFile__
#define __ASSFile__

#include "ASSLine.h"
#include <vector>
#include <string>
#include <fstream>

class ASSFile
{
private:
	std::vector <std::string> preprocess;
	std::vector <ASSLine> lines;
	std::string title;
	int uniqueLines;

	void readPreprocessLines(std::ifstream &fin);
	void processLyricLines(std::ifstream &fin);

public:
	ASSFile(std::string inFileName);
	void removeAllTags();
	void removeNonKaraokeTags();



};

#endif