#ifndef __ASSLineWithSwitch__
#define __ASSLineWithSwitch__

#include "ASSTime.h"
#include "SwitchCodes.h"
#include "ASSLine.h"
#include <string>
#include <vector>

class ASSLineWithSwitch : public ASSLine
{
private:	
	std::vector <StyleSwitchCode> styleSwitchCodes;
	std::vector <KaraokeSwitchCode> karaokeSwitchCodes;

	std::vector <KaraokeSwitchCode> getKaraokeSwitchCodes(std::string input);

	ASSLineWithSwitch processSameStartTime(std::vector <ASSLine> &input);
	ASSLineWithSwitch processSameEndTime(std::vector <ASSLine> &input);

	void fixDurationFromStart();
	void fixDurationFromEnd();

	ASSTime getAggregateStyleSwitchDuration(int index);
	ASSTime getAggregateKaraokeSwitchDuration(int index);

	std::string printKaraokeLine();

public:
	// Constructors
	ASSLineWithSwitch();
	ASSLineWithSwitch(std::string input);
	ASSLineWithSwitch(ASSLine input);
	ASSLineWithSwitch(std::vector <ASSLine> input);

	// Operators
	void operator=(ASSLineWithSwitch input);

	// Setters
	void setStart(ASSTime toSet);
	void setEnd(ASSTime toSet);

	// Returns a string representation of the object
	std::string printASSLine();

	ASSLineWithSwitch mergeStyles(ASSLineWithSwitch toMerge);
};

#endif