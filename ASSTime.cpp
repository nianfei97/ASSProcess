#include "ASSTime.h"

#include <sstream>

ASSTime::ASSTime()
{
	hour = 0;
	min = 0;
	sec = 0;
	ms = 0;
}

ASSTime::ASSTime(std::string input)
{
	std::istringstream iss(input);

	std::string temp;

	getline(iss, temp, ':');
	hour = stoi(temp);
	getline(iss, temp, ':');
	min = stoi(temp);
	getline(iss, temp, '.');
	sec = stoi(temp);
	getline(iss, temp);
	ms = stoi(temp);
}

std::string ASSTime::printASSTime()
{
	std::ostringstream sout;

	sout << hour << ':';

	if (min < 10) sout << '0';
	sout << min << ':';

	if (sec < 10) sout << '0';
	sout << sec << '.';

	if (ms < 10) sout << '0';
	sout << ms;

	return sout.str();
}

void ASSTime::operator=(ASSTime input)
{
	this->hour = input.hour;
	this->min = input.min;
	this->sec = input.sec;
	this->ms = input.ms;
}

void ASSTime::operator=(std::string input)
{
	ASSTime temp(input);
	*this = temp;
}

ASSTime ASSTime::operator+(ASSTime toAdd)
{
	ASSTime result;

	result.hour = this->hour + toAdd.hour;
	result.min = this->min + toAdd.min;
	result.sec = this->sec + toAdd.sec;
	result.ms = this->ms + toAdd.ms;

	result.round();

	return result;
}

void ASSTime::operator+=(ASSTime toAdd)
{
	*this = *this + toAdd;
}

ASSTime ASSTime::operator-(ASSTime toSubtract)
{
	ASSTime result;

	result.hour = this->hour - toSubtract.hour;
	result.min = this->min - toSubtract.min;
	result.sec = this->sec - toSubtract.sec;
	result.ms = this->ms - toSubtract.ms;

	result.round();

	return result;
}

void ASSTime::operator-=(ASSTime  toSubtract)
{
	*this = *this + toSubtract;
}

void ASSTime::round()
{
	if (this->ms >= 100)
	{
		this->sec += 1;
		this->ms -= 100;
	}

	if (this->sec >= 60)
	{
		this->min += 1;
		this->sec -= 60;
	}

	if (this->min >= 60)
	{
		this->hour += 1;
		this->min -= 60;
	}

	if (this->ms < 0)
	{
		this->sec -= 1;
		this->ms += 100;
	}

	if (this->sec < 0)
	{
		this->min -= 1;
		this->sec += 60;
	}

	if (this->min < 0)
	{
		this->hour -= 1;
		this->min += 60;
	}

	if (this->hour < 0)
	{
		ASSTime blank;
		*this = blank;
	}
}