#include "ASSTime.h"
#include <sstream>

ASSTime::ASSTime()
{
	this->hour = 0;
	this->min = 0;
	this->sec = 0;
	this->cs = 0;
}

ASSTime::ASSTime(std::string input)
{
	std::istringstream iss(input);

	std::string temp;

	getline(iss, temp, ':');
	this->hour = stoi(temp);
	getline(iss, temp, ':');
	this->min = stoi(temp);
	getline(iss, temp, '.');
	this->sec = stoi(temp);
	getline(iss, temp);
	this->cs = stoi(temp);
}

ASSTime::ASSTime(int input)
{
	this->cs = input % 100;
	input /= 100;

	this->sec = input % 60;
	input /= 60;

	this->min = input % 60;
	input /= 60;

	this->hour = input;
}

std::string ASSTime::printASSTime()
{
	std::ostringstream sout;

	sout << hour << ':';

	if (min < 10) sout << '0';
	sout << min << ':';

	if (sec < 10) sout << '0';
	sout << sec << '.';

	if (cs < 10) sout << '0';
	sout << cs;

	return sout.str();
}

void ASSTime::operator=(ASSTime input)
{
	this->hour = input.hour;
	this->min = input.min;
	this->sec = input.sec;
	this->cs = input.cs;
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
	result.cs = this->cs + toAdd.cs;

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
	result.cs = this->cs - toSubtract.cs;

	result.round();

	return result;
}

void ASSTime::operator-=(ASSTime  toSubtract)
{
	*this = *this + toSubtract;
}

bool ASSTime::operator==(ASSTime toCompare)
{
	return (*this <= toCompare) && (*this >= toCompare);
}

bool ASSTime::operator!=(ASSTime toCompare)
{
	return !(*this == toCompare);
}

bool ASSTime::operator<(ASSTime toCompare)
{
	return this->getDurationCS() < toCompare.getDurationCS();
}

bool ASSTime::operator>(ASSTime toCompare)
{
	return this->getDurationCS() > toCompare.getDurationCS();
}

bool ASSTime::operator<=(ASSTime toCompare)
{
	return !(*this > toCompare);
}

bool ASSTime::operator>=(ASSTime toCompare)
{
	return !(*this < toCompare);
}

void ASSTime::round()
{
	if (this->cs >= 100)
	{
		this->sec += 1;
		this->cs -= 100;
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

	if (this->cs < 0)
	{
		this->sec -= 1;
		this->cs += 100;
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

int ASSTime::getDurationCS()
{
	int output = 0;

	output += this->cs;
	output += this->sec * 100;
	output += this->min * 100 * 60;
	output += this->hour * 100 * 60 * 60;

	return output;
}