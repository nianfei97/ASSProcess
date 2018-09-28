#include "SongFile.h"
#include "ASSFile.h"

#include <iostream>
#include <string>

#define STYLED_PATH "test\\Colored\\"
#define EN_PATH "test\\EN\\"
#define K_PATH "test\\Karaoke\\"

std::string convertWhitespaceToUnderscore(std::string input);
std::string removeExtension(std::string input);

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Please include input file and styles..." << std::endl;
        return 1;
    }

    std::string setlistFileName = argv[1];
    std::string stylesFileName = argv[2];

    try
    {
        std::cout << "Loading setlist..." << std::endl;
        ASSFileWithSwitch setlist(setlistFileName);

        ASSFileWithSwitch output;
        output.readPreprocessLines(stylesFileName);

        for (ASSLine currSong : setlist.getLines())
        {
            if (!currSong.isComment())
            {
                std::string songTitle = currSong.getText();
                std::string parsedSongTitle = convertWhitespaceToUnderscore(songTitle);

                std::cout << "Processing " << songTitle << "..." << std::endl;

                SongFile currSongFile(STYLED_PATH + parsedSongTitle + ".ass",
                                      K_PATH + parsedSongTitle + ".ass",
                                      EN_PATH + parsedSongTitle + ".txt",
                                      stylesFileName);

                ASSTime offset = currSong.getStart() - currSongFile.getStart();
                currSongFile.offsetFile(offset);

                output.appendFile(currSongFile.getFile());
            }
        }

        std::string outputFileName = removeExtension(setlistFileName) + "_OUT.ass";
        output.printASSFile(outputFileName);
    }
    catch (const char* exception)
    {
        std::cerr << exception << std::endl;
    }

    return 0;
}

std::string convertWhitespaceToUnderscore(std::string input)
{
    std::string output = "";

    for (char c : input)
    {
        if (c == ' ') output += '_';
        else output += c;
    }

    return output;
}

std::string removeExtension(std::string input)
{
    std::string output = input;

    for (int i = 0; i < 4; i++)
        output.pop_back();

    return output;
}