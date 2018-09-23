#ifndef __SWITCHCODES__
#define __SWITCHCODES__

#include "ASSTime.h"
#include <string>

class StyleSwitchCode
{
private:
	ASSTime duration;
	std::string style;

public:
	StyleSwitchCode();
	StyleSwitchCode(ASSTime duration, std::string style);

	ASSTime getDuration();
	std::string getStyle();
};

class KSwitchCode
{
private:
	ASSTime duration;
	std::string textFragment;
};

#endif