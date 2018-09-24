#ifndef __ASSLine__
#define __ASSLine__

#include "ASSTime.h"
#include "SwitchCodes.h"
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

	std::string removeAllTags(std::string input);
	std::string removeNonKaraokeTags(std::string input);

public:
	// Construtors
	ASSLine();
	ASSLine(std::string input);

	// Operators
	void operator= (ASSLine input);
	ASSLine operator+ (ASSLine toAdd);
	void operator+= (ASSLine toAdd);

	// Setters
	void setStart(ASSTime toSet);
	void setEnd(ASSTime toSet);
	void setText(std::string toSet);
	void setStyle(std::string toSet);
	void setLayer(int toSet);

	// Getters
	ASSTime getStart();
	ASSTime getEnd();
	std::string getText();
	std::string getStyle();
	int getLayer();

	// Returns the duration of the object
	ASSTime getDuration();

	// Returns a string representation of the object
	std::string printASSLine();

	// Remove all tags from the object's text field
	void removeAllTags();

	// Removes all non-karaoke tags from the object's text field
	void removeNonKaraokeTags();
};

class ASSLineWithSwitch : public ASSLine
{
private:	
	std::vector <StyleSwitchCode> styleSwitchCodes;
	std::vector <KaraokeSwitchCode> karaokeSwitchCodes;

	ASSLineWithSwitch processSameStartTime (std::vector <ASSLine> &input);
	ASSLineWithSwitch processSameEndTime (std::vector <ASSLine> &input);
	ASSTime getAggregateSwitchDuration (int index);
	std::vector <KaraokeSwitchCode> getKaraokeSwitchCodes (std::string input);
	std::string printKaraokeLine();

public:
	// Constructors
	ASSLineWithSwitch(std::string input);
	ASSLineWithSwitch(ASSLine input);
	ASSLineWithSwitch(std::vector <ASSLine> input);

	// Operators
	void operator=(ASSLineWithSwitch input);

	// Returns a string representation of the object
	std::string printASSLine();
};

#endif