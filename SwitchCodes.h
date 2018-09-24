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

class KaraokeSwitchCode
{
private:
	ASSTime duration;
	std::string textFragment;

public:
	KaraokeSwitchCode();
	KaraokeSwitchCode(ASSTime duration, std::string textFragment);

	ASSTime getDuration();
	std::string getTextFragment();
};

#endif