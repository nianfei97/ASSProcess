#include "ASSLine.h"
#include <sstream>

#define ONETIME_ZERO_STRING "0:00:00.00"
#define DEFAULT_STYLE "Default"

#define DEFAULT_TAG "{\\fad(200,200)\\be11}"
#define UP_TAG "{\\fad(200,200)\\be11\\an8}"
#define TITLE_TAG "{\\fad(200,200)\\be11\\an7}"

ASSLine::ASSLine()
{
	this->start = ONETIME_ZERO_STRING;
	this->end = ONETIME_ZERO_STRING;
	this->text = "";
	this->style = DEFAULT_STYLE;
	this->actor = "";
	this->leftMargin = 0;
	this->rightMargin = 0;
	this->verticalMargin = 0;
	this->effect = "";
	this->layer = 0;
	this->isComment = false;
	this->isUp = false;
	this->isDialogue = false;
}

ASSLine::ASSLine(std::string input)
{
	std::istringstream iss(input);

	std::string formatString;
	getline(iss, formatString, ' ');
	if (formatString == "Comment:") this->isDialogue = true;

	std::string layerString;
	getline(iss, layerString, ',');
	this->layer = stoi(layerString);

	std::string startString;
	getline(iss, startString, ',');
	this->start = startString;

	std::string endString;
	getline(iss, endString, ',');
	this->end = endString;

	std::string styleString;
	getline(iss, styleString, ',');
	this->style = styleString;

	std::string actorString;
	getline(iss, actorString, ',');
	this->actor = actorString;

	std::string leftMarginString;
	getline(iss, leftMarginString, ',');
	this->leftMargin = stoi(leftMarginString);

	std::string rightMarginString;
	getline(iss, rightMarginString, ',');
	this->rightMargin = stoi(rightMarginString);

	std::string verticalMarginString;
	getline(iss, verticalMarginString, ',');
	this->verticalMargin = stoi(verticalMarginString);

	std::string effectString;
	getline(iss, effectString, ',');
	this->effect = effectString;

	std::string textString;
	getline(iss, textString);
	this->text = textString;

	this->isComment = false;

	if (styleString[0] == 'U') 
	{
		this->isUp = true;
		this->style = this->style.substr(1, this->style.length() - 1);
	}
}

void ASSLine::operator= (ASSLine input)
{
	this->start = input.start;
	this->end = input.end;
	this->text = input.text;
	this->style = input.style;
	this->actor = input.actor;
	this->leftMargin = input.leftMargin;
	this->rightMargin = input.rightMargin;
	this->verticalMargin = input.verticalMargin;
	this->effect = input.effect;
	this->layer = input.layer;
	this->isComment = input.isComment;
	this->isUp = input.isUp;
}

ASSLine ASSLine::operator+(ASSLine toAdd)
{
	ASSLine sum;

	sum.start = this->start;
	sum.end = toAdd.end;
	sum.text = this->text + ' ' + toAdd.text;
	sum.style = this->style;
	sum.layer = this->layer;

	return sum;
}

void ASSLine::operator+=(ASSLine toAdd)
{
	*this = *this + toAdd;
}

int ASSLine::getLayer()
{
	return this->layer;
}

std::string ASSLine::printLine()
{
	std::ostringstream sout;

	if (this->isComment) sout << "Comment: ";
	else sout << "Dialogue: ";

	sout << this->layer << ',';
	sout << this->start.printASSTime() << ',';
	sout << this->end.printASSTime() << ',';
	sout << this->style << ',';
	sout << this->actor << ',';
	sout << this->leftMargin << ',';
	sout << this->rightMargin << ',';
	sout << this->verticalMargin << ',';
	sout << this->effect << ',';


	if (!this->isComment)
	{
		if (this->isUp) sout << UP_TAG;
		else if (this->style == "Title") sout << TITLE_TAG;
		else if (this->isDialogue); // Do nothing
		else sout << DEFAULT_TAG;
	}

	sout << this->text;

	return sout.str();
}

void ASSLine::removeAllTags()
{
	std::string result = "";
	bool writeEnable = true;

	for (int i = 0; i < this->text.size(); i++)
	{
		if (this->text[i] == '{')
		{
			writeEnable = false;
			i++;
		}
		else if (this->text[i] == '}')
		{
			writeEnable = true;
			i++;
		}
		
		if (writeEnable)
			result += this->text[i];
	}

	this->text = result;
}

void ASSLine::removeNonKaraokeTags()
{
	std::string result = "";
	bool writeEnable = true;

	for (int i = 0; i < this->text.size(); i++)
	{
		if (this->text[i] == '{')
		{
			if (this->text[i + 2] != 'k' && this->text[i + 2] != 'K')
			{
				writeEnable = false;
				i++;
			}
		}
		else if (this->text[i] == '}' && !writeEnable)
		{
			writeEnable = true;
			i++;
		}
		
		if (writeEnable)
			result += this->text[i];
	}

	this->text = result;
}