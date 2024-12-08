#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <stack>

using CalibrationEquation = std::pair<uint64_t, std::vector<int>>;

bool IsEquationValid(uint64_t testValue, uint64_t previousEvaluation, std::vector<int>::iterator currentIterator, std::vector<int>::iterator endIterator, std::stack<char> &operations)
{
    uint64_t value = *currentIterator;

    uint64_t multipliedValue = previousEvaluation * value;
    uint64_t addedValue = previousEvaluation + value;

    std::vector<int>::iterator nextIterator = (currentIterator);
    nextIterator++;

    if (nextIterator == endIterator)
    {
        if (multipliedValue == testValue)
        {
            operations.push('*');
            return true;
        }
        else if (addedValue == testValue)
        {
            operations.push('+');
            return true;
        }

        return false;
    }
    else
    {
        if (multipliedValue > 0 && multipliedValue <= testValue)
        {
            if (IsEquationValid(testValue, multipliedValue, nextIterator, endIterator, operations))
            {
                operations.push('*');
                return true;
            }
        }

        if (addedValue <= testValue)
        {
            if (IsEquationValid(testValue, addedValue, nextIterator, endIterator, operations))
            {
                operations.push('+');
                return true;
            }
        }
    }

    return false;
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

    std::vector<CalibrationEquation> equations;

    // Parse map (upside-down for parsing : will need to reverse it afterwards)
    std::string currentLine;
    while (std::getline(inFile, currentLine))
    {
        std::istringstream lineStream(currentLine);

        unsigned long long testValue;

        if (!(lineStream >> testValue))
        {
            std::cerr << "Failure to parse test value on equation : " << currentLine << std::endl;
            return -1;
        }

        if (lineStream.get() != ':')
        {
            std::cerr << "Unexpected character when expecting delimiter character ':' in line : " << currentLine << std::endl;
            return -2;
        }

        std::vector<int> equationNumbers;
        int equationNumber;
        while (lineStream >> equationNumber)
        {
            equationNumbers.push_back(equationNumber);
        }

        equations.push_back({testValue, equationNumbers});
    }

    // // DEBUG
    // std::cout << "printing equations : \n\n";
    // for (auto equation : equations)
    // {
    //     std::cout << equation.first << ": ";
    //     for (auto number : equation.second)
    //     {
    //         std::cout << number << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // Exercise 1
    {
        uint64_t validTestValuesAccumulator = 0;

        for (auto equation : equations)
        {
            uint64_t testValue = equation.first;
            std::vector<int> numbers = equation.second;
            std::stack<char> operations;

            if (IsEquationValid(testValue, 0, numbers.begin(), numbers.end(), operations))
            {
                operations.pop();

                std::cout << testValue;

                std::cout << " : ";
                for (auto nb : numbers)
                {
                    char operation = '\n';
                    if (!operations.empty())
                    {
                        operation = operations.top();
                        operations.pop();
                    }

                    std::cout << nb << operation;
                }

                std::cout << std::endl;

                validTestValuesAccumulator += testValue;
            }
        }

        std::cout << "\n\n" << "Exercise 1 result :\n" << validTestValuesAccumulator << "\n\n\n";
    }

    // Exercise 2
    {
        int validNewObstaclePositionNumber = 0;

        std::cout << "\n\n" << "Exercise 2 result :\n" << validNewObstaclePositionNumber << "\n\n\n";
    }

    return 0;
}