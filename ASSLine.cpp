#include "ASSLine.h"
#include "SwitchCodes.h"

#include <sstream>
#include <iostream>

#define ONETIME_ZERO_STRING "0:00:00.00"
#define ONETIME_FIVE_STRING "0:00:05.00"
#define DEFAULT_STYLE "Default"

#define DEFAULT_TAG "{\\fad(200,200)\\be11}"
#define UP_TAG "{\\fad(200,200)\\be11\\an8}"
#define TITLE_TAG "{\\fad(200,200)\\be11\\an7}"
#define DIALOGUE_TAG "{\\rAvenir Next LT Pro\\fs60}"

ASSLine::ASSLine()
{
	this->start = ONETIME_ZERO_STRING;
	this->end = ONETIME_FIVE_STRING;
	this->text = "";
	this->style = DEFAULT_STYLE;
	this->actor = "";
	this->leftMargin = 0;
	this->rightMargin = 0;
	this->verticalMargin = 0;
	this->effect = "";
	this->layer = 0;
	this->iscomment = false;
	this->isup = false;
	this->isdialogue = false;
}

ASSLine::ASSLine(std::string input) : ASSLine()
{
	std::istringstream iss(input);

	std::string formatString;
	getline(iss, formatString, ' ');
	if (formatString == "Comment:") this->iscomment = true;

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
	while (textString.back() == ' ') textString.pop_back();
	this->text = textString;
}

void ASSLine::operator=(ASSLine input)
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
	this->iscomment = input.isComment();
	this->isup = input.isUp();
	this->isdialogue = input.isDialogue();
}

int ASSLine::getLayer()
{
	return this->layer;
}

ASSTime ASSLine::getStart()
{
	return this->start;
}

ASSTime ASSLine::getEnd()
{
	return this->end;
}

std::string ASSLine::getText()
{
	return this->text;
}

std::string ASSLine::getStyle()
{
	return this->style;
}

bool ASSLine::isComment()
{
	return this->iscomment;
}

bool ASSLine::isUp()
{
	return this->isup;
}

bool ASSLine::isDialogue()
{
	return this->isdialogue;
}

void ASSLine::setStart(ASSTime toSet)
{
	this->start = toSet;
}

void ASSLine::setEnd(ASSTime toSet)
{
	this->end = toSet;
}

void ASSLine::setText(std::string toSet)
{
	this->text = toSet;
}

void ASSLine::setStyle(std::string toSet)
{
	this->style = toSet;
}

void ASSLine::setLayer(int toSet)
{
	this->layer = toSet;
}

void ASSLine::setComment()
{
	this->iscomment = true;
}

void ASSLine::unsetComment()
{
	this->iscomment = false;
}

void ASSLine::setUp()
{
	this->isup = true;
}

void ASSLine::unsetUp()
{
	this->isup = false;
}

void ASSLine::setDialogue()
{
	this->isdialogue = true;
}

void ASSLine::unsetDialogue()
{
	this->isdialogue = false;
}

ASSTime ASSLine::getDuration()
{
	return this->end - this->start;
}

std::string ASSLine::printASSLine()
{
	std::ostringstream sout;

	if (this->isComment()) sout << "Comment: ";
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

	if (!this->isComment())
	{
		if (this->isUp()) sout << UP_TAG;
		else if (this->style == "Title") sout << TITLE_TAG;
		else if (this->isDialogue()) sout << DIALOGUE_TAG;
		else sout << DEFAULT_TAG;
	}

	sout << this->text;
	sout << std::endl;

	return sout.str();
}

std::string ASSLine::removeAllTags(std::string input)
{
	std::string output = "";
	bool writeEnable = true;

	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == '{')
		{
			writeEnable = false;
			i++;
		}
		else if (input[i] == '}')
		{
			writeEnable = true;
			i++;
		}
		
		if (writeEnable)
			output += input[i];
	}

	return output;
}

void ASSLine::removeAllTags()
{
	this->text = removeAllTags(this->text);
}

std::string ASSLine::removeNonKaraokeTags(std::string input)
{
	std::string output = "";
	bool writeEnable = true;

	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == '{')
		{
			if (input[i + 2] != 'k' && input[i + 2] != 'K')
			{
				writeEnable = false;
				i++;
			}
		}
		else if (input[i] == '}' && !writeEnable)
		{
			writeEnable = true;
			i++;
		}
		
		if (writeEnable)
			output += input[i];
	}

	return output;
}

void ASSLine::removeNonKaraokeTags()
{
	this->text = removeNonKaraokeTags(this->text);
}

void ASSLine::offsetASSLine(ASSTime duration)
{
	this->start += duration;
	this->end += duration;
}
