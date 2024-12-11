#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <cmath>

#include <map>

#include <chrono>

struct StoneEntry
{
    uint64_t nextPotentialValue;
    uint64_t nextPotentialValueIfSplit = 0;
    uint64_t nbOfStones = 0;
    bool willSplit = false;
};

std::pair<uint64_t, uint64_t> GetNextPotentialStoneValues(uint64_t initialStoneValue, bool & willSplit)
{
    uint64_t nextPotentialValue;
    uint64_t nextPotentialValueIfSplit = 0;
    willSplit = false;

    if (initialStoneValue == 0)
    {
        nextPotentialValue = 1;
    }
    else if (int digitNb = log10(initialStoneValue) + 1; digitNb % 2 == 0)
    {
        int middleDigitIndex = digitNb / 2;
        int powerOfTenSplitValue = pow(10, middleDigitIndex);
        nextPotentialValue = initialStoneValue / powerOfTenSplitValue;
        nextPotentialValueIfSplit = initialStoneValue % powerOfTenSplitValue;

        willSplit = true;
    }
    else
    {
        nextPotentialValue = initialStoneValue * 2024;
    }

    return {nextPotentialValue, nextPotentialValueIfSplit};
}

void AddStonesToNextValueInStoneInfos(uint64_t nextValue, uint64_t numberOfStones, std::map<uint64_t, StoneEntry> & newStoneInfos)
{
    if (auto search = newStoneInfos.find(nextValue); search != newStoneInfos.end())
    {
        search->second.nbOfStones += numberOfStones;
    }
    else
    {
        bool willSplit = false;
        auto newNextStoneValues = GetNextPotentialStoneValues(nextValue, willSplit);
        newStoneInfos[nextValue] = {newNextStoneValues.first, newNextStoneValues.second, numberOfStones, willSplit};
    }
}

int main(int argc, char *argv[])
{
    char *inputFileName;
    if (argc <= 2)
    {
        std::cout << "Provide input filename AND number of blinks (optionally enable exercise 1 via -exo1)" << std::endl;
        return 1;
    }

    inputFileName = argv[1];

    std::ifstream inFile(inputFileName);
    if (!inFile.is_open())
    {
        std::cout << "failed to open " << inputFileName << '\n';
        return 2;
    }

    int blinksNb = -1;
    try
    {
        blinksNb = std::stoi(argv[2]);
    }
    catch(const std::invalid_argument & invalidArgumentException)
    {
        std::cerr << "Could not parse number of blinks : " << invalidArgumentException.what() << '\n';
        return 3;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    bool enableExo1 = false;
    if (argc > 3 && std::string(argv[3]) == "-exo1")
        enableExo1 = true;

    std::vector<uint64_t> stones;
    uint64_t stone;

    // Parse data
    while (inFile >> stone)
    {
        stones.push_back(stone);
    }

    // // DEBUG
    // std::cout << "printing stones :\n";
    // for (auto stone : stones)
    // {
    //     std::cout << stone << " ";
    // }
    // std::cout << std::endl;

    // Exercise 1 
    // (Ultra brute force method : not usable for part two)
    if (enableExo1)
    {
        std::vector<uint64_t> stonesExo1(stones);
        int blinksNbExo1 = blinksNb;

        auto start = std::chrono::high_resolution_clock::now();

        while (blinksNbExo1 > 0)
        {
            // DEBUG
            std::cout << "Blink : " << blinksNbExo1 << std::endl;
        
            for (int i = 0; i < stonesExo1.size(); i++)
            {
                uint64_t stoneValue = stonesExo1[i];
                if (stoneValue == 0)
                {
                    stonesExo1[i] = 1;
                    continue;
                }
                else if (int digitNb = log10(stoneValue) + 1; digitNb % 2 == 0)
                {
                    int middleDigitIndex = digitNb / 2;
                    int powerOfTenSplitValue = pow(10, middleDigitIndex);
                    int p1 = stoneValue / powerOfTenSplitValue;
                    int p2 = stoneValue % powerOfTenSplitValue;

                    auto posIter = stonesExo1.begin() + i;
                    stonesExo1[i] = p2;
                    stonesExo1.insert(posIter, p1);
                    i++;
                }
                else
                {
                    stonesExo1[i] *= 2024;
                }
            }

            blinksNbExo1--;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // // DEBUG
        // for (auto stone : stones)
        // {
        //     std::cout << stone << " ";
        // }
        // std::cout << "\n\n\n";

        std::cout << "\n\n" << "Exercise 1 result :\n" << stonesExo1.size() << "\n\n\n";
        std::cout << "\n\n" << "Time to complete :\n" << (duration) << "us"<< "\n\n\n";
    }

    // Exercise 2
    {
        auto start = std::chrono::high_resolution_clock::now();

        // Initialize stone infos
        std::map<uint64_t, StoneEntry> stoneInfosPerStoneValue;
        for (auto stoneValue : stones)
        {
            bool willSplit = false;
            std::pair<uint64_t,uint64_t> nextStoneValues = GetNextPotentialStoneValues(stoneValue, willSplit);
            stoneInfosPerStoneValue[stoneValue] = {nextStoneValues.first, nextStoneValues.second, 1, willSplit};
        }
        std::map<uint64_t, StoneEntry> newStoneInfos(stoneInfosPerStoneValue);

        while (blinksNb > 0)
        {
            // // DEBUG
            // std::cout << "Blink : " << blinksNb << std::endl;

            for (auto &stoneInfo : stoneInfosPerStoneValue)
            {
                uint64_t currentStoneValue = stoneInfo.first;
                StoneEntry oldStoneEntry = stoneInfo.second;
                uint64_t numberOfStones = oldStoneEntry.nbOfStones;

                if (oldStoneEntry.willSplit)
                    AddStonesToNextValueInStoneInfos(oldStoneEntry.nextPotentialValueIfSplit, numberOfStones, newStoneInfos);
                
                AddStonesToNextValueInStoneInfos(oldStoneEntry.nextPotentialValue, numberOfStones, newStoneInfos);

                newStoneInfos[currentStoneValue].nbOfStones -= numberOfStones;
            }

            stoneInfosPerStoneValue = newStoneInfos;

            blinksNb--;
        }

        uint64_t finalNbOfStones = 0;
        for (auto const &stoneInfo : stoneInfosPerStoneValue)
        {
            uint64_t previousNbOfStones = finalNbOfStones;
            finalNbOfStones += stoneInfo.second.nbOfStones;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        
        std::cout << "\n\n" << "Exercise 2 result :\n" << finalNbOfStones << "\n\n\n";
        std::cout << "\n\n" << "Time to complete :\n" << (duration) << "us"<< "\n\n\n";
    }

    return 0;
}