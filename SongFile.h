#include "ASSFile.h"
#include "ENLyricFile.h"

#include <string>
#include <vector>

class SongFile
{
private:
	ASSFileWithSwitch data;

	void configureStyles(ASSFileWithSwitch& input);
	bool canFindUnmatchedLine(ASSFileWithSwitch& file1, ASSFileWithSwitch& file2);
	ASSFileWithSwitch mergeStylesIntoFile(ASSFileWithSwitch& original, ASSFileWithSwitch& toMerge);
	void checkNumberOfLines(ASSFileWithSwitch& ASS, ENLyricFile& EN);

	ASSLineWithSwitch createDividerLine(std::string input, ASSTime startTime, ASSTime endTime);
	ASSLineWithSwitch createMetadataLine(std::vector <std::string> input, ASSTime startTime, ASSTime endTime);
	ASSLineWithSwitch createENLine(ASSLineWithSwitch JPline, std::string ENtext);

public:
	SongFile(std::string styleFileName, std::string karaokeFileName, std::string ENFileName, std::string metadataFileName);
	void printFile(std::string outFileName);
	
	ASSFileWithSwitch getFile();
	ASSTime getStart();
	ASSTime getEnd();

	void offsetFile(ASSTime offset);
};