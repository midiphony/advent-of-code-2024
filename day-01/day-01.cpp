#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

int main(int argc, char *argv[])
{
    std::cout << "Welcome to Advent of Code 2024 - day 1 !" << std::endl;

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

    std::vector<int> firstList;
    std::vector<int> secondList;

    int firstNumber, secondNumber;
    while (infile >> firstNumber >> secondNumber)
    {
        firstList.push_back(firstNumber);
        secondList.push_back(secondNumber);
    }

    std::sort(firstList.begin(), firstList.end());
    std::sort(secondList.begin(), secondList.end());

    for (const int& i : firstList)
    {
        std::cout << i << std::endl;
    }

    std::cout << std::endl
              << std::endl;

    for (const int& i : secondList)
    {
        std::cout << i << std::endl;
    }

    int firstListSize = firstList.size();
    if (firstListSize != secondList.size())
    {
        std::cout << "The two lists have two different sizes : " << firstListSize << " and " << secondList.size() << std::endl;
        return 3;
    }

    long totalDistance = 0;
    for (int i = 0; i < firstListSize; i++)
    {
        totalDistance += std::abs(firstList[i] - secondList[i]);
    }

    std::cout << std::endl << "TOTAL DISTANCE IS : " << std::endl << totalDistance << std::endl;

    int totalSimilarity = 0;
    for (const int& locId : firstList)
    {
        int occurence = 0;

        for (const int& j : secondList)
        {
            if (locId == j)
                occurence++;
        }

        totalSimilarity += locId * occurence;
    }

    std::cout << std::endl << "TOTAL SIMILARITY IS : " << std::endl << totalSimilarity << std::endl;

    return 0;
}