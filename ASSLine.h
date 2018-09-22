#ifndef __ASSLine__
#define __ASSLine__

#include "ASSTime.h"
#include <string>

class ASSLine
{
private:
	ASSTime start;
	ASSTime end;
	std::string text;
	std::string style;
	std::string actor;
	int leftMargin;
	int rightMargin;
	int verticalMargin;
	std::string effect;
	int layer;
	bool isComment;
	bool isUp;
	bool isDialogue;

public:
	ASSLine();

	ASSLine(std::string input);

	void operator= (ASSLine input);

	ASSLine operator+ (ASSLine toAdd);

	void operator+= (ASSLine toAdd);

	int getLayer();

	// Returns formatted std::string for printing given a ASSLine input
	std::string printLine();

	void removeAllTags();

	void removeNonKaraokeTags();
};

#endif