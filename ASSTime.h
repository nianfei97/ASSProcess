#ifndef __ASSTime__
#define __ASSTime__

#include <string>

class ASSTime
{
private:
	int hour;
	int min;
	int sec;
	int ms;

public:
	ASSTime();
	ASSTime(std::string input);
	std::string printASSTime();
	void operator=(ASSTime input);
	void operator=(std::string input);
	ASSTime operator+(ASSTime toAdd);
	void operator+=(ASSTime toAdd);
	ASSTime operator-(ASSTime toSubtract);
	void operator-=(ASSTime toSubtract);
	void round();
};

#endif