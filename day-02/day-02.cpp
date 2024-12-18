#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>

bool GetLevelsLine(std::ifstream & inFile, std::vector<int> & levels)
{
    std::string levelsLine;
    levels.clear();
    if (std::getline(inFile, levelsLine))
    {
        int currentLevel;

        levels.clear();

        std::istringstream iss(levelsLine);
        while(iss >> currentLevel)
        {
            levels.push_back(currentLevel);
        }

        return true;
    }

    return false;
}

bool IsReportSafeByDefault(std::vector<int> const & levels)
{
    if (levels.size() <= 0)
        return false;
    else if (levels.size() == 1)
        return true;

    int currentLevel = levels[0];
    bool isAscending = levels[1] - levels[0] > 0;

    int i;
    for (i = 1; i < levels.size(); i++)
    {
        int nextLevel = levels[i];

        int diff = nextLevel - currentLevel;
        if (diff == 0 
            || diff > 0 && false == isAscending
            || diff < 0 && true == isAscending)
            break; // unsafe report : early exit

        int absDiff = std::abs(diff);
        if (absDiff > 3)
            break; // unsafe report : early exit

        currentLevel = nextLevel;
    }

    //if (currentLevel == levels[levels.size() - 1])
    if (i == levels.size())
        return true;

    return false;
}

// Brute forcing safety check by skipping a level
bool IsReportSafeWithTolerance(std::vector<int> const & levels, int & skippedIndex)
{
    if (levels.size() <= 0)
        return false;
    else if (levels.size() == 1)
        return true;

    std::vector<int> tolerantLevels(levels.size() - 1);

    for (skippedIndex = 0; skippedIndex < levels.size(); skippedIndex++)
    {
        tolerantLevels.clear();

        for (int i = 0; i < levels.size(); i++)
        {
            if (i != skippedIndex)
                tolerantLevels.push_back(levels[i]);
        }

        if (IsReportSafeByDefault(tolerantLevels))
            break;
    }

    if (skippedIndex >= levels.size())
        return false;

    return true;
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

    std::ifstream infile(inputFileName);
    if (!infile.is_open())
    {
        std::cout << "failed to open " << inputFileName << '\n';
        return 2;
    }

    // Exercise 1
    std::vector<int> levels;
    int safeReportsNumber = 0;
    while (GetLevelsLine(infile, levels))
    {
        if (IsReportSafeByDefault(levels))
            safeReportsNumber++;
    }

    std::cout << "\n\n" << "Safe reports number : " << safeReportsNumber << "\n\n\n";

    // Exercise 2
    infile.close();
    infile.open(inputFileName);

    int tolerantReports = 0;
    while (GetLevelsLine(infile, levels))
    {
        int skippedIndex;

        if (IsReportSafeByDefault(levels))
            tolerantReports++;
        else if (IsReportSafeWithTolerance(levels, skippedIndex))
        {
            tolerantReports++;

            std::cout << "Is Safe with tolerance : ";
            for (const int & level : levels)
                std::cout << level << " ";
            std::cout << "   -----   " << "unsafe level : " << levels[skippedIndex] << std::endl;
        }
        else
        {
            std::cout << "NOT SAFE : ";
            for (const int & level : levels)
                std::cout << level << " ";
            std::cout << std::endl;
        }
    }

    

    std::cout << "\n\n" << "Safe (WITH TOLERANCE) reports number : " << tolerantReports << std::endl;


    return 0;
}