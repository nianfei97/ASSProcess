#ifndef __ENLyricFile__
#define __ENLyricFile__

#include <string>
#include <vector>

class ENLyricFile
{
private:
	std::vector <std::string> metadata;
	std::vector <std::string> lines;

public:
	ENLyricFile(std::string inFile);

	int getNumLines();
	std::string getLine(int index);
	std::string getTitle();
	std::vector <std::string> getMetadata();
};

#endif