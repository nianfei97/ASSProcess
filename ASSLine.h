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
	bool iscomment;
	bool isup;
	bool isdialogue;

	std::string removeAllTags(std::string input);
	std::string removeNonKaraokeTags(std::string input);

public:
	// Construtors
	ASSLine();
	ASSLine(std::string input);

	// Operators
	void operator=(ASSLine input);

	// Setters
	void setStart(ASSTime toSet);
	void setEnd(ASSTime toSet);
	void setText(std::string toSet);
	void setStyle(std::string toSet);
	void setLayer(int toSet);
	void setComment();
	void unsetComment();
	void setUp();
	void unsetUp();
	void setDialogue();
	void unsetDialogue();

	// Getters
	ASSTime getStart();
	ASSTime getEnd();
	std::string getText();
	std::string getStyle();
	int getLayer();
	bool isComment();
	bool isUp();
	bool isDialogue();

	// Returns the duration of the object
	ASSTime getDuration();

	// Returns a string representation of the object
	std::string printASSLine();

	// Remove all tags from the object's text field
	void removeAllTags();

	// Removes all non-karaoke tags from the object's text field
	void removeNonKaraokeTags();

	// Offsets line by specified duration
	void offsetASSLine(ASSTime duration);
};

#endif