#include "ASSFile.h"
#include <iostream>

using namespace std;

int main()
{
	ASSFileWithSwitch file("test\\Karaoke\\Koi_ni_Naritai_AQUARIUM.ass");

	ofstream fout("out.txt");

	for (ASSLineWithSwitch currLine : file.getLines())
		fout << currLine.getText() << endl;

	return 0;
}