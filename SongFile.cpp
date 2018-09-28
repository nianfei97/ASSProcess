#include "SongFile.h"
#include "ASSFile.h"
#include "ENLyricFile.h"

#include <iostream>

#define DIVIDER_STYLE "-- DIVIDER --"
#define METADATA_STYLE "Title"

const ASSTime ZERO("0:00:00.00");
const ASSTime METADATA_DURATION("0:00:05.00");

SongFile::SongFile(std::string styleFileName, std::string karaokeFileName, std::string ENFileName, std::string metadataFileName)
{
	try
	{
		ASSFileWithSwitch withK(karaokeFileName);
		ASSFileWithSwitch withStyle(styleFileName);

		ASSFileWithSwitch merged = mergeStylesIntoFile(withK, withStyle);
		configureStyles(merged);

		ENLyricFile EN(ENFileName);
		checkNumberOfLines(merged, EN);

		ASSFileWithSwitch output;
		output.readPreprocessLines(metadataFileName);

		output.insertLine(createDividerLine(EN.getTitle(), ZERO, merged.getEndTime()));
		output.insertLine(createMetadataLine(EN.getMetadata(), ZERO, merged.getEndTime()));

		output.insertLine(createDividerLine("English", ZERO, merged.getEndTime()));

		for (int i = 0; i < merged.getNumLines(); i++)
			output.insertLine(createENLine(merged.getLine(i), EN.getLine(i)));

		output.insertLine(createDividerLine("Romaji", ZERO, merged.getEndTime()));

		for (ASSLineWithSwitch currLine : merged.getLines())
		{
			ASSLineWithSwitch toAdd = currLine;
			output.insertLine(toAdd);

			if (toAdd.isUp())
				output.swapLines(output.getNumLines() - 1, output.getNumLines() - merged.getNumLines() - 2);
		}

		this->data = output;
	}
	catch (const char* exception)
	{
		throw exception;
	}
}

void SongFile::configureStyles(ASSFileWithSwitch& input)
{
	for (int i = 0; i < input.getNumLines(); i++)
	{
		if (input.getLine(i).isComment())
		{
			ASSLineWithSwitch toSet = input.getLine(i);
			toSet.unsetComment();
			toSet.setDialogue();

			input.setLine(toSet, i);
		}

		if (input.getLine(i).getStyle()[0] == 'U')
		{
			ASSLineWithSwitch toSet = input.getLine(i);

			toSet.setStyle(toSet.getStyle().substr(1, toSet.getStyle().length() - 1));
			toSet.setUp();

			input.setLine(toSet, i);
		}
	}
}

ASSFileWithSwitch SongFile::mergeStylesIntoFile(ASSFileWithSwitch& original, ASSFileWithSwitch& toMerge)
{
	if (!canFindUnmatchedLine(original, toMerge))
	{
		try
		{
			ASSFileWithSwitch output = toMerge;
			output.clearLines();

			for (int i = 0; i < original.getNumLines(); i++)
				output.insertLine(original.getLine(i).mergeStyles(toMerge.getLine(i)));

			return output;
		}
		catch (const char* exception)
		{
			throw exception;
		}
	}
	else
	{
		throw "Cannot merge styles!";
	}
}

bool SongFile::canFindUnmatchedLine(ASSFileWithSwitch& file1, ASSFileWithSwitch& file2)
{
	for (int i = 0; i < std::min(file1.getNumLines(), file2.getNumLines()); i++)
	{
		if (file1.getLine(i).getText() != file2.getLine(i).getText())
		{
			std::cout << "Unmatched line at line " << i + 1<< std::endl;
			std::cout << file1.getLine(i).getText() << std::endl;
			std::cout << file2.getLine(i).getText() << std::endl;

			return true;
		}	
	}

	return false;
}

void SongFile::checkNumberOfLines(ASSFileWithSwitch& ASS, ENLyricFile& EN)
{
	if (ASS.getNumLines() != EN.getNumLines())
	{
		std::cerr << "ASS file has " << ASS.getNumLines() << " lines." << std::endl;
		std::cerr << "EN file has " << EN.getNumLines() << " lines." << std::endl;
		throw "Mismatch in number of lines!";
	}
}

ASSLineWithSwitch SongFile::createDividerLine(std::string input, ASSTime startTime, ASSTime endTime)
{
	ASSLineWithSwitch result;

	result.setStart(startTime);
	result.setEnd(endTime);
	result.setStyle(DIVIDER_STYLE);
	result.setText(input);
	result.setComment();

	return result;
}

ASSLineWithSwitch SongFile::createMetadataLine(std::vector <std::string> input, ASSTime startTime, ASSTime endTime)
{
	ASSLineWithSwitch result;

	result.setStart(startTime);
	result.setEnd(startTime + METADATA_DURATION);
	result.setStyle(METADATA_STYLE);

	std::string text = "";

	for (std::string currLine : input)
	{
		text += currLine;
		text += "\\N"; 
	}

	result.setText(text);

	return result;
}

ASSLineWithSwitch SongFile::createENLine(ASSLineWithSwitch JPline, std::string ENtext)
{
	ASSLineWithSwitch result = JPline;

	result.clearKaraokeSwitch();
	result.setStyle("EN" + JPline.getStyle().substr(2, JPline.getStyle().length() - 2));

	std::vector <StyleSwitchCode> styleCodes;

	for (StyleSwitchCode currCode : JPline.getStyleSwitchCodes())
	{
		StyleSwitchCode currStyleCode(currCode.getDuration(), "EN" + currCode.getStyle().substr(2, currCode.getStyle().length() - 2));
		styleCodes.push_back(currStyleCode);
	}

	result.setStyleSwitchCodes(styleCodes);
	result.setText(ENtext);

	if (JPline.getText() == ENtext)
		result.setComment();

	return result;
}

void SongFile::printFile(std::string outFileName)
{
	this->data.printASSFile(outFileName);
}

ASSFileWithSwitch SongFile::getFile()
{
	return this->data;
}

ASSTime SongFile::getStart()
{
	return this->data.getLine(3).getStart();
}

ASSTime SongFile::getEnd()
{
	return this->data.getLine(data.getNumLines() - 1).getEnd();
}

void SongFile::offsetFile(ASSTime offset)
{
	this->data.offsetFile(offset);
}