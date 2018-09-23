#ifndef __ASSLine__
#define __ASSLine__

#include "ASSTime.h"
#include <string>
#include <vector>

class ASSLine
{
protected:
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
	// Construtors
	ASSLine();
	ASSLine(std::string input);

	// Operators
	void operator= (ASSLine input);
	ASSLine operator+ (ASSLine toAdd);
	void operator+= (ASSLine toAdd);

	// Getters
	ASSTime getStart();
	ASSTime getEnd();
	std::string getText();
	std::string getStyle();
	int getLayer();

	// Setters
	void setStart(ASSTime toSet);
	void setEnd(ASSTime toSet);
	void setText(std::string toSet);
	void setStyle(std::string toSet);
	void setLayer(int toSet);

	// Returns the duration of the object
	ASSTime getDuration();

	// Returns a string representation of the object
	std::string printLine();

	// Remove all tags from the object's text field
	void removeAllTags();

	// Removes all non-karaoke tags from the object's text field
	void removeNonKaraokeTags();
};

class ASSLineWithSwitch : ASSLine
{
private:	
	std::vector <ASSTime> switchDurations;
	std::vector <std::string> switchStyles;

	ASSLineWithSwitch processSameStartTime (std::vector <ASSLine> &input);
	ASSLineWithSwitch processSameEndTime (std::vector <ASSLine> &input);
	ASSTime getAggregateDuration (int index);
public:
	// Constructors
	ASSLineWithSwitch(ASSLine input);
	ASSLineWithSwitch(std::vector <ASSLine> input);

	// Returns a string representation of the object
	std::string printLine();
};

#endif