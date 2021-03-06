#include "ASSLineWithSwitch.h"

#include <sstream>
#include <iostream>

#define KARAOKE_TAG_START "{\\k"
#define KARAOKE_TAG_END "}"

#define INF 1000000000

ASSLineWithSwitch::ASSLineWithSwitch() : ASSLine()
{
	this->createDefaultStyleSwitchCode();
	this->createDefaultKaraokeSwitchCode();
}

ASSLineWithSwitch::ASSLineWithSwitch(std::string input) : ASSLine(input)
{
	this->createDefaultStyleSwitchCode();

	this->karaokeSwitchCodes = getKaraokeSwitchCodes(this->text);
	this->text = removeAllTags(this->text);
}

ASSLineWithSwitch::ASSLineWithSwitch(ASSLine input)
{
	this->ASSLine::operator=(input);
	this->createDefaultStyleSwitchCode();

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
}

void ASSLineWithSwitch::operator=(ASSLineWithSwitch input)
{
	this->ASSLine::operator=(input);
	this->styleSwitchCodes = input.styleSwitchCodes;
	this->karaokeSwitchCodes = input.karaokeSwitchCodes;
}

void ASSLineWithSwitch::createDefaultStyleSwitchCode()
{
	this->styleSwitchCodes.clear();
	StyleSwitchCode currStyleSwitchCode(this->getDuration(), this->getStyle());
	this->styleSwitchCodes.push_back(currStyleSwitchCode);
}

void ASSLineWithSwitch::createDefaultKaraokeSwitchCode()
{
	this->karaokeSwitchCodes.clear();
}

void ASSLineWithSwitch::setStart(ASSTime toSet)
{
	this->ASSLine::setStart(toSet);
	this->fixDurationFromStart();
}

void ASSLineWithSwitch::fixDurationFromStart()
{
	while (this->styleSwitchCodes.size() > 1 && this->getDuration() < this->getAggregateStyleSwitchDuration(0, this->styleSwitchCodes.size() - 1))
	{
		this->styleSwitchCodes.erase(this->styleSwitchCodes.begin());
	}

	if (this->styleSwitchCodes.size() > 1)
	{
		this->styleSwitchCodes.begin()->setDuration(this->getDuration() - this->getAggregateStyleSwitchDuration(1, this->styleSwitchCodes.size() - 1));
	}
	else
	{
		this->createDefaultStyleSwitchCode();
	}

	while (this->getDuration() > this->getAggregateKaraokeSwitchDuration(0, this->karaokeSwitchCodes.size() - 1) && this->karaokeSwitchCodes.size() > 0)
	{
		this->karaokeSwitchCodes.erase(this->karaokeSwitchCodes.begin());
	}

	if (this->karaokeSwitchCodes.size() > 1)
	{
		this->karaokeSwitchCodes.begin()->setDuration(this->getDuration() - this->getAggregateKaraokeSwitchDuration(1, this->karaokeSwitchCodes.size() - 1));
	}
	else
	{
		this->createDefaultKaraokeSwitchCode();
	}
}

void ASSLineWithSwitch::setEnd(ASSTime toSet)
{
	this->ASSLine::setEnd(toSet);
	this->fixDurationFromEnd();
}

void ASSLineWithSwitch::fixDurationFromEnd()
{
	while (this->styleSwitchCodes.size() > 1 && this->getDuration() < this->getAggregateStyleSwitchDuration(0, this->styleSwitchCodes.size() - 1))
	{
		this->styleSwitchCodes.pop_back();
	}

	if (this->styleSwitchCodes.size() > 1)
	{
		this->styleSwitchCodes.rbegin()->setDuration(this->getDuration() - this->getAggregateStyleSwitchDuration(0, this->styleSwitchCodes.size() - 2));
	}
	else
	{
		this->createDefaultStyleSwitchCode();
	}

	while (this->karaokeSwitchCodes.size() > 1 && this->getDuration() < this->getAggregateKaraokeSwitchDuration(0, this->karaokeSwitchCodes.size() - 1))
	{
		this->karaokeSwitchCodes.pop_back();
	}

	if (this->karaokeSwitchCodes.size() > 1)
	{
		this->karaokeSwitchCodes.rbegin()->setDuration(this->getDuration() - this->getAggregateKaraokeSwitchDuration(0, this->karaokeSwitchCodes.size() - 2));
	}
	else
	{
		this->createDefaultKaraokeSwitchCode();
	}
}

void ASSLineWithSwitch::setStyle(std::string toSet)
{
	this->ASSLine::setStyle(toSet);
	this->styleSwitchCodes.begin()->setStyle(toSet);
}

void ASSLineWithSwitch::setStyleSwitchCodes(std::vector <StyleSwitchCode> toSet)
{
	this->styleSwitchCodes = toSet;
}

std::vector <StyleSwitchCode> ASSLineWithSwitch::getStyleSwitchCodes()
{
	return this->styleSwitchCodes;
}

ASSLineWithSwitch ASSLineWithSwitch::processSameStartTime(std::vector <ASSLine> &input)
{
	ASSLineWithSwitch result = ASSLineWithSwitch(*input.rbegin());
	result.styleSwitchCodes.clear();

	if (input.size() == 1)
	{
		result.createDefaultStyleSwitchCode();
	}
	else
	{
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
	}

	return result;
}

ASSLineWithSwitch ASSLineWithSwitch::processSameEndTime(std::vector <ASSLine> &input)
{
	ASSLineWithSwitch result = ASSLineWithSwitch(*input.begin());
	result.styleSwitchCodes.clear();

	if (input.size() == 1)
	{
		result.createDefaultStyleSwitchCode();
	}
	else
	{
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
	}

	return result;
}

ASSTime ASSLineWithSwitch::getAggregateStyleSwitchDuration(int start, int end)
{
	ASSTime result;

	for (int i = start; i <= end; i++)
		result += this->styleSwitchCodes[i].getDuration();

	return result;
}

ASSTime ASSLineWithSwitch::getAggregateKaraokeSwitchDuration(int start, int end)
{
	ASSTime result;

	for (int i = start; i <= end; i++)
		result += this->karaokeSwitchCodes[i].getDuration();

	return result;
}

std::string ASSLineWithSwitch::printASSLine()
{
	std::ostringstream sout;

	for (int i = 0; i < this->styleSwitchCodes.size(); i++)
	{
		ASSLine toPrint = *this;
		toPrint.setEnd(toPrint.getStart() + this->getAggregateStyleSwitchDuration(0, i));
		toPrint.setStyle(this->styleSwitchCodes[i].getStyle());
		toPrint.setLayer(styleSwitchCodes.size() - i - 1);
		toPrint.setText(this->printKaraokeLine());

		sout << toPrint.printASSLine();
	}

	return sout.str();
}

std::vector <KaraokeSwitchCode> ASSLineWithSwitch::getKaraokeSwitchCodes(std::string input)
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

			char currTextFragment[input.size()];

			iss.get(currTextFragment, input.size(), '{');

			KaraokeSwitchCode currSwitchCode(currDuration, currTextFragment);
			output.push_back(currSwitchCode);
		}
	}

	return output;
}

std::string ASSLineWithSwitch::printKaraokeLine()
{
	std::ostringstream output;

	if (this->karaokeSwitchCodes.empty())
	{
		output << this->getText();
	}
	else
	{
		for (KaraokeSwitchCode currCode : this->karaokeSwitchCodes)
		{
			output << KARAOKE_TAG_START << currCode.getDuration().getDurationCS() << KARAOKE_TAG_END;
			output << currCode.getTextFragment();
		}
	}

	return output.str();
}

ASSLineWithSwitch ASSLineWithSwitch::mergeStyles(ASSLineWithSwitch toMerge)
{
	ASSLineWithSwitch output = *this;

	if (toMerge.isComment())
		output.setComment();

	output.setStyle(toMerge.getStyle());
	output.styleSwitchCodes.clear();

	if (!output.karaokeSwitchCodes.empty())
	{
		if (toMerge.styleSwitchCodes.size() > 1)
		{
			for (int i = 0; i < toMerge.styleSwitchCodes.size(); i++)
			{
				int closestDiff = INF;
				int closestIndex = -1;

				for (int j = 0; j < output.karaokeSwitchCodes.size(); j++)
				{
					int currDiff = abs(output.getAggregateKaraokeSwitchDuration(0, j).getDurationCS() - toMerge.getAggregateStyleSwitchDuration(0, i).getDurationCS());

					if (currDiff < closestDiff)
					{
						closestDiff = currDiff;
						closestIndex = j;
					}
				}

				char response = ' ';

				if (i != toMerge.styleSwitchCodes.size() - 1)
				{
					while (1)
					{
						for (int j = 0; j <= closestIndex; j++)
							std::cout << output.karaokeSwitchCodes[j].getTextFragment();

						std::cout << " | ";

						for (int j = closestIndex + 1; j < output.karaokeSwitchCodes.size(); j++)
								std::cout << output.karaokeSwitchCodes[j].getTextFragment();

						std::cout << std::endl;

						std::cin >> response;

						if (response == 'c')
							break;
						else if (response == 'v')
							closestIndex++;
						else if (response == 'x')
							closestIndex--;
						else
							std::cout << "Unrecognised Command!" << std::endl;		
					}
				}

				ASSTime switchStyleDuration;

				if (i == 0)
					switchStyleDuration = output.getAggregateKaraokeSwitchDuration(0, closestIndex);
				else if (i == toMerge.styleSwitchCodes.size() - 1)
					switchStyleDuration = output.getDuration() - output.getAggregateStyleSwitchDuration(0, i - 1);
				else
					switchStyleDuration = output.getAggregateKaraokeSwitchDuration(0, closestIndex) - output.getAggregateStyleSwitchDuration(0, i - 1);

				StyleSwitchCode result(switchStyleDuration, toMerge.styleSwitchCodes[i].getStyle());
				output.styleSwitchCodes.push_back(result);
			}
		}
		else
		{
			output.createDefaultStyleSwitchCode();
		}
	}
	else
	{
		output.createDefaultStyleSwitchCode();
	}

	return output;
}

void ASSLineWithSwitch::clearStyleSwitch()
{
	this->createDefaultStyleSwitchCode();
}

void ASSLineWithSwitch::clearKaraokeSwitch()
{
	this->createDefaultKaraokeSwitchCode();
}

void ASSLineWithSwitch::offsetLine(ASSTime offset)
{
	this->start += offset;
	this->end += offset;
}