#include "ASSLine.h"
#include "SwitchCodes.h"

#include <sstream>
#include <iostream>

#define ONETIME_ZERO_STRING "0:00:00.00"
#define DEFAULT_STYLE "Default"

#define KARAOKE_TAG_START "{\\k"
#define KARAOKE_TAG_END "}"

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

ASSLine::ASSLine(std::string input) : ASSLine()
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
	while (textString.back() == ' ') textString.pop_back();
	this->text = textString;

	this->isComment = false;

	if (styleString[0] == 'U') 
	{
		this->isUp = true;
		this->style = this->style.substr(1, this->style.length() - 1);
	}
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

ASSTime ASSLine::getDuration()
{
	return this->end - this->start;
}

std::string ASSLine::printASSLine()
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

ASSLineWithSwitch::ASSLineWithSwitch(std::string input) : ASSLine(input)
{
	StyleSwitchCode currStyleSwitchCode(this->getDuration(), this->getStyle());
	this->styleSwitchCodes.push_back(currStyleSwitchCode);

	this->karaokeSwitchCodes = getKaraokeSwitchCodes(this->text);
	this->text = removeAllTags(this->text);
}

ASSLineWithSwitch::ASSLineWithSwitch(ASSLine input)
{
	this->ASSLine::operator=(input);

	StyleSwitchCode currStyleSwitchCode(this->getDuration(), this->getStyle());
	this->styleSwitchCodes.push_back(currStyleSwitchCode);
	
	this->karaokeSwitchCodes = getKaraokeSwitchCodes(this->text);
	this->text = removeAllTags(this->text);
}

ASSLineWithSwitch::ASSLineWithSwitch(std::vector <ASSLine> input)
{
	std::string lineText = input[0].getText();
	ASSTime startTime = input[0].getStart();
	ASSTime endTime = input[0].getEnd();

	for (ASSLine line : input)
		if (line.getText() != lineText)
			throw "Lines contain different text fields!";

	bool hasSameStartTime = true;
	bool hasSameEndTime = true;

	for (ASSLine line : input)
	{
		if (line.getStart() != startTime) hasSameStartTime = false;
		if (line.getEnd() != endTime) hasSameEndTime = false;
	}

	if (hasSameStartTime)
	{
		ASSLineWithSwitch result = processSameStartTime(input);
		*this = result;
	}
	else if (hasSameEndTime)
	{
		ASSLineWithSwitch result = processSameEndTime(input);
		*this = result;
	}
	else throw "Unrecognised timing format!";

	this->karaokeSwitchCodes = getKaraokeSwitchCodes(this->text);
	this->text = removeAllTags(this->text);

}

void ASSLineWithSwitch::operator=(ASSLineWithSwitch input)
{
	this->ASSLine::operator=(input);
	this->styleSwitchCodes = input.styleSwitchCodes;
}

ASSLineWithSwitch ASSLineWithSwitch::processSameStartTime (std::vector <ASSLine> &input)
{
	ASSLineWithSwitch result = ASSLineWithSwitch(*input.rbegin());

	for (int i = 0; i < input.size(); i++)
	{
		ASSTime switchDuration;
		std::string switchStyle;

		if (i == 0) switchDuration = input[i].getDuration();
		else if (i > 0) switchDuration = input[i].getDuration() - input[i - 1].getDuration();

		switchStyle = input[i].getStyle();

		StyleSwitchCode toPush = StyleSwitchCode(switchDuration, switchStyle);

		result.styleSwitchCodes.push_back(toPush);
	}

	return result;
}

ASSLineWithSwitch ASSLineWithSwitch::processSameEndTime (std::vector <ASSLine> &input)
{
	ASSLineWithSwitch result = ASSLineWithSwitch(*input.begin());

	for (int i = 0; i < input.size(); i++)
	{
		ASSTime switchDuration;
		std::string switchStyle;

		if (i < input.size() - 1) switchDuration = input[i].getDuration() - input[i + 1].getDuration();
		else if (i == input.size() - 1) switchDuration = input[i].getDuration();
		
		switchStyle = input[i].getStyle();

		StyleSwitchCode toPush = StyleSwitchCode(switchDuration, switchStyle);

		result.styleSwitchCodes.push_back(toPush);
	}

	return result;
}

ASSTime ASSLineWithSwitch::getAggregateSwitchDuration (int index)
{
	ASSTime result;

	for (int i = 0; i <= index; i++)
		result += this->styleSwitchCodes[i].getDuration();

	return result;
}

std::string ASSLineWithSwitch::printASSLine()
{
	std::ostringstream sout;

	for (int i = 0; i < this->styleSwitchCodes.size(); i++)
	{
		ASSLine toPrint = *this;
		toPrint.setEnd(toPrint.getStart() + this->getAggregateSwitchDuration(i));
		toPrint.setStyle(this->styleSwitchCodes[i].getStyle());
		toPrint.setLayer(styleSwitchCodes.size() - i - 1);
		toPrint.setText(this->printKaraokeLine());

		sout << toPrint.printASSLine();
	}

	return sout.str();
}

//{\k34}Shi{\k27}ning {\k29}Road {\k15}ha{\k44}shi{\k32}ri{\k30}da{\k30}su {\k31}ko{\k27}no {\k30}ki{\k15}mo{\k78}chi

std::vector <KaraokeSwitchCode> ASSLineWithSwitch::getKaraokeSwitchCodes (std::string input)
{
	std::vector <KaraokeSwitchCode> output;

	std::istringstream iss(this->removeNonKaraokeTags(input));

	while (!iss.eof())
	{
		char currChar = iss.get();

		if (currChar == '{')
		{
			iss.get();
			iss.get();

			int currDurationInt;
			iss >> currDurationInt;

			ASSTime currDuration(currDurationInt);

			iss.get();

			char* currTextFragment;

			iss.get(currTextFragment, input.size(), '{');

			KaraokeSwitchCode currSwitchCode(currDuration, currTextFragment);
			output.push_back(currSwitchCode);
		}
		else
		{
			throw "Karaoke Switch Code Error";
		}
	}

	return output;
}

std::string ASSLineWithSwitch::printKaraokeLine()
{
	std::ostringstream output;

	for (KaraokeSwitchCode currCode : this->karaokeSwitchCodes)
	{
		output << KARAOKE_TAG_START << currCode.getDuration().getDurationCS() << KARAOKE_TAG_END;
		output << currCode.getTextFragment();
	}

	return output.str();
}