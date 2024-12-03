#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>

// Return position immediately after next integer, or std::npos if failure to parse
int GetPositionAfterNextInteger(std::string const &line, int const &position, int &parsedInteger)
{
    if (std::isdigit(line[position])) // "mult(" => 4 char
    {
        int numberFirstIndex = position;
        int numberLastIndex = numberFirstIndex + 1;

        while (numberLastIndex < line.size() && std::isdigit(line[numberLastIndex]))
        {
            numberLastIndex++;
        }

        parsedInteger = stoi(line.substr(numberFirstIndex, numberLastIndex - numberFirstIndex));
        return numberLastIndex;
    }

    return std::string::npos;
}

int main(int argc, char *argv[])
{
    std::cout << "Welcome to Advent of Code 2024 - day 2 !" << std::endl;

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

    // Exercise 1
    {
        std::string currentLine;
        int accumulator = 0;
        while (std::getline(inFile, currentLine))
        {
            int lastMulIndex = 0;
            while (lastMulIndex < currentLine.size())
            {
                lastMulIndex = currentLine.find("mul(", lastMulIndex);
                
                // No mul op anymore : go to next line
                if (lastMulIndex == std::string::npos)
                {
                    break;
                }

                int firstNumber;
                int posAfterNextInteger = GetPositionAfterNextInteger(currentLine, lastMulIndex + 4, firstNumber);
                if (posAfterNextInteger != std::string::npos && posAfterNextInteger < currentLine.size() && currentLine[posAfterNextInteger] == ',')
                {
                    int secondNumber;
                    posAfterNextInteger = GetPositionAfterNextInteger(currentLine, posAfterNextInteger + 1, secondNumber);

                    if (posAfterNextInteger != std::string::npos && posAfterNextInteger < currentLine.size() && currentLine[posAfterNextInteger] == ')')
                    {
                        accumulator += firstNumber * secondNumber;

                        std::cout << firstNumber << "\t*\t" << secondNumber << "\t=\t" << (firstNumber * secondNumber) << std::endl;
                    }
                }

                lastMulIndex += 4;
            }
        }    

        std::cout << "\n\n" << "Exercise 1 result :\n" << accumulator << "\n\n\n";
    }

    // Exercise 2
    {
        std::string currentLine;
        int accumulator = 0;
        bool mulOpEnabled = true;

        inFile.close();
        inFile.open(inputFileName);

        while (std::getline(inFile, currentLine))
        {
            int cursorIndex = 0;
            while (cursorIndex < currentLine.size())
            {
                if (false == mulOpEnabled)
                {
                    cursorIndex = currentLine.find("do()", cursorIndex);

                    // No do() : go to next line
                    if (cursorIndex == std::string::npos)
                    {
                        break;
                    }

                    mulOpEnabled = true;

                    cursorIndex += 4; // 4 == length of "do()"

                    // If end of line, go to next line
                    if (cursorIndex >= currentLine.size())
                    {
                        break;
                    }
                }

                int nextMulIndex = currentLine.find("mul(", cursorIndex);
                int nextDontIndex = currentLine.find("don't()", cursorIndex);

                if (nextDontIndex != std::string::npos && nextMulIndex != std::string::npos)
                {
                    if (nextDontIndex < nextMulIndex)
                    {
                        mulOpEnabled = false;
                        cursorIndex = nextDontIndex + 7; // 7 == length of "don't()"
                        continue;
                    }
                    else
                    {
                        cursorIndex = nextMulIndex;
                    }
                }
                // No dont anymore in this line
                else if (nextDontIndex == std::string::npos)
                {
                    cursorIndex = nextMulIndex;
                }
                // No mul anymore, but a dont !
                else if (nextDontIndex == std::string::npos)
                {
                    mulOpEnabled = false;
                    cursorIndex = nextDontIndex + 7; // 7 == length of "don't()"
                    continue;
                }

                // No mul op anymore : go to next line
                if (cursorIndex == std::string::npos)
                {
                    break;
                }

                int firstNumber;
                int posAfterNextInteger = GetPositionAfterNextInteger(currentLine, cursorIndex + 4, firstNumber); // +4 == length of "mul("
                if (posAfterNextInteger != std::string::npos && posAfterNextInteger < currentLine.size() && currentLine[posAfterNextInteger] == ',')
                {
                    int secondNumber;
                    posAfterNextInteger = GetPositionAfterNextInteger(currentLine, posAfterNextInteger + 1, secondNumber);

                    if (posAfterNextInteger != std::string::npos && posAfterNextInteger < currentLine.size() && currentLine[posAfterNextInteger] == ')')
                    {
                        accumulator += firstNumber * secondNumber;

                        std::cout << firstNumber << "\t*\t" << secondNumber << "\t=\t" << (firstNumber * secondNumber) << std::endl;
                    }
                }

                cursorIndex += 4;
            }
        }    

        std::cout << "\n\n" << "Exercise 2 result :\n" << accumulator << "\n\n\n";
    }


    return 0;
}