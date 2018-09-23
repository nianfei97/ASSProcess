#include "SwitchCodes.h"

#include "ASSTime.h"
#include <string>

StyleSwitchCode::StyleSwitchCode()
{

}

StyleSwitchCode::StyleSwitchCode(ASSTime duration, std::string style)
{
	this->duration = duration;
	this->style = style;
}

ASSTime StyleSwitchCode::getDuration()
{
	return this->duration;
}

std::string StyleSwitchCode::getStyle()
{
	return this->style;
}