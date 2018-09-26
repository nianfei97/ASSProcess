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

void StyleSwitchCode::setDuration(ASSTime toSet)
{
	this->duration = toSet;
}

void StyleSwitchCode::setStyle(std::string toSet)
{
	this->style = toSet;
}

KaraokeSwitchCode::KaraokeSwitchCode()
{

}
	
KaraokeSwitchCode::KaraokeSwitchCode(ASSTime duration, std::string textFragment)
{
	this->duration = duration;
	this->textFragment = textFragment;
}

ASSTime KaraokeSwitchCode::getDuration()
{
	return this->duration;
}

void KaraokeSwitchCode::setDuration(ASSTime toSet)
{
	this->duration = toSet;
}

std::string KaraokeSwitchCode::getTextFragment()
{
	return this->textFragment;
}
