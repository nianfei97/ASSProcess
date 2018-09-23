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

	// Ensures each time field in the object is within bounds
	void round();

public:
	// Construtors
	ASSTime();
	ASSTime(std::string input);

	// Operators
	void operator=(ASSTime input);
	void operator=(std::string input);
	ASSTime operator+(ASSTime toAdd);
	void operator+=(ASSTime toAdd);
	ASSTime operator-(ASSTime toSubtract);
	void operator-=(ASSTime toSubtract);
	bool operator==(ASSTime toCompare);
	bool operator!=(ASSTime toCompare);

	// Returns a string representation of the object
	std::string printASSTime();
};

#endif