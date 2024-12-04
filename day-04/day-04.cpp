#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>

enum Direction
{
    Right,
    Left,
    Up,
    Down,
    UpRight,
    DownRight,
    DownLeft,
    UpLeft
};

const std::string WORD_TO_FIND_PART_ONE = "XMAS";
const std::string WORD_TO_FIND_PART_TWO = "MAS";

bool IsLetterPresentAt(std::vector<std::string> const & lines, int x, int y, int index, Direction direction, const std::string wordToFind)
{
    int xi = x;
    int yi = y;

    // Right / left
    if (direction == Right || direction == UpRight || direction == DownRight)
        xi += index;
    else if (direction == Left || direction == UpLeft || direction == DownLeft)
        xi -= index;

    // Up / down
    if (direction == Up || direction == UpRight || direction == UpLeft)
        yi += index;
    else if (direction == Down || direction == DownRight || direction == DownLeft)
        yi -= index;

    if (yi < 0 || yi >= lines.size())
        return false;
    
    if (xi < 0 || xi >= lines[yi].size())
        return false;
    
    if (lines[yi][xi] != wordToFind[index])
        return false;

    return true;
}

bool IsWordPresentAt(std::vector<std::string> const & lines, int x, int y, Direction direction, const std::string wordToFind)
{
    int wordToFindLength = wordToFind.size();
    int i = 0;
    for (i = 0; i < wordToFindLength; i++)
    {
        if (false == IsLetterPresentAt(lines, x, y, i, direction, wordToFind))
            break;
    }

    if (i == wordToFindLength)
    {
        return true;
    }

    return false;
}

int GetNumberOfWordOccurencesAt(std::vector<std::string> const & lines, int x, int y, const std::string wordToFind)
{
    int numberOfXmasWord = 0;
    int i = 0;

    // ASSUMING ENUM IS ORDERED ONE BY ONE, WITH "RIGHT" AS FIRST VALUE AND "UPLEFT" AS LAST VALUE
    for (int directionIndex = Right; directionIndex <= UpLeft; directionIndex++)
    {
        Direction direction = static_cast<Direction>(directionIndex);
        if (IsWordPresentAt(lines, x, y, direction, wordToFind))
        {
            numberOfXmasWord++;
        }
    }

    // // DEBUG
    // if (numberOfXmasWord > 0)
    // {
    //     std::cout << x << "\t" << y << "\t" << "\t occurrences =\t" << numberOfXmasWord << std::endl;
    //     getchar();
    // }

    return numberOfXmasWord;
}

int main(int argc, char *argv[])
{
    char *inputFileName;
    if (argc <= 1)
    {
        std::cout << "Provide an input filename as argument." << std::endl;
        return 1;
    }

    inputFileName = argv[1];

    std::ifstream inFile(inputFileName);
    if (!inFile.is_open())
    {
        std::cout << "failed to open " << inputFileName << '\n';
        return 2;
    }
    
    std::string currentLine;
    std::vector<std::string> lines;
    while (std::getline(inFile, currentLine))
    {
        lines.push_back(std::move(currentLine));
    }

    // Exercise 1
    {
        int numberOfXmasWord = 0;
        for (int y = 0; y < lines.size(); y++)
        {
            for (int x = 0; x < lines[y].size(); x++)
            {
                numberOfXmasWord += GetNumberOfWordOccurencesAt(lines, x, y, WORD_TO_FIND_PART_ONE);
            }
        }

        std::cout << "\n\n" << "Exercise 1 result :\n" << numberOfXmasWord << "\n\n\n";
    }

    // Exercise 2
    // We must find the following pattern, in whatever orientation :
    // M.S
    // .A.
    // M.S
    {
        int numberOfXmasPattern = 0;
        for (int y = 0; y < lines.size(); y++)
        {
            for (int x = 0; x < lines[y].size(); x++)
            {
                // Early exit if there's no A
                if (lines[y][x] != 'A')
                {
                    std::cout << ".";
                    continue;
                }

                int topLeftCornerX = x - 1;
                int topLeftCornerY = y - 1;

                if (topLeftCornerY < 0 || topLeftCornerY + 2 >= lines.size())
                {
                    std::cout << ".";
                    continue;
                }

                if (topLeftCornerX < 0 || topLeftCornerX + 2 >= lines[x].size()) // Not bullet-proof : we assume all lines are equal length here
                {
                    std::cout << ".";
                    continue;
                }

                int localNumberOfXmas = 0;
                // Assuming a MAS pattern will count 2-MAS
                if (IsWordPresentAt(lines, x - 1, y - 1, UpRight, WORD_TO_FIND_PART_TWO))
                    localNumberOfXmas++;
                if (IsWordPresentAt(lines, x + 1, y - 1, UpLeft, WORD_TO_FIND_PART_TWO))
                    localNumberOfXmas++;
                if (IsWordPresentAt(lines, x - 1, y + 1, DownRight, WORD_TO_FIND_PART_TWO))
                    localNumberOfXmas++;
                if (IsWordPresentAt(lines, x + 1, y + 1, DownLeft, WORD_TO_FIND_PART_TWO))
                    localNumberOfXmas++;
                
                if (localNumberOfXmas == 2)
                {
                    numberOfXmasPattern++;
                    std::cout << "X";
                    //std::cout << "Found X-MAS pattern at\t" << x << "\t" << y << std::endl;
                }
                else if (localNumberOfXmas > 2)
                {
                    std::cout << "?";
                    return 20;
                }
                else
                {
                    std::cout << ".";
                }
            }

            std::cout << std::endl;
        }

        std::cout << "\n\n" << "Exercise 2 result :\n" << numberOfXmasPattern << "\n\n\n";
    }

    return 0;
}