#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <map>
#include <set>

// #include <string>

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

    // Exercise 1
    {
        int validUpdatesMiddlePageNumberSum = 0;

        std::map<int, std::set<int>> priorityRules;

        std::string currentLine;
        
        int priorNumber;
        int laterNumber;
        
        // Parse priority rules
        while (inFile >> priorNumber && inFile.get() == '|' && inFile >> laterNumber)
        {
            std::set<int> &laterNumberToPriorityNumber = priorityRules[priorNumber];
            laterNumberToPriorityNumber.insert(laterNumber);

            if (inFile.get() == '\n' && inFile.peek() == '\n')
                break; // end of priority rules
        }

        // Parse updates
        std::vector<std::vector<int>> updatesList;
        while (inFile.peek() != EOF)
        {
            int newNumber;
            std::vector<int> update;
            while (inFile >> newNumber && inFile.get() == ',')
                update.push_back(newNumber);

            update.push_back(newNumber);

            updatesList.push_back(update);
        }

        for(auto update : updatesList)
        {
            bool isUpdateInvalid = false;

            for (int firstNumberIndex = 0; firstNumberIndex < update.size() - 1; firstNumberIndex++)
            {
                int firstNumber = update[firstNumberIndex];

                for (std::vector<int>::iterator laterNumberIterator = update.begin() + firstNumberIndex + 1; laterNumberIterator != update.end() && false == isUpdateInvalid; laterNumberIterator++)
                {
                    int laterNumber = *laterNumberIterator;

                    std::map<int, std::set<int>>::iterator kvpFromPriorityRule = priorityRules.find(laterNumber);
                    if (kvpFromPriorityRule != priorityRules.end() && kvpFromPriorityRule->second.find(firstNumber) != kvpFromPriorityRule->second.end())
                    {
                        isUpdateInvalid = true;

                        // DEBUG :
                        std::cout << "Following update is invalid : ";
                        for (int updateNumber : update)
                            std::cout << updateNumber << ",";

                        std::cout << "\t because of numbers : " << laterNumber << "|" << firstNumber << std::endl;
                    }
                }
            }

            if (false == isUpdateInvalid)
            {
                int middleValue = *(update.begin() + update.size()/2);

                std::cout << "Valid : ";
                for (int updateNumber : update)
                    std::cout << updateNumber << ",";
                std::cout << "\t middle : " << middleValue << std::endl;

                validUpdatesMiddlePageNumberSum += middleValue;
            }
        }

        std::cout << "\n\n" << "Exercise 1 result :\n" << validUpdatesMiddlePageNumberSum << "\n\n\n";
    }

    // Exercise 2
    {

        // std::cout << "\n\n" << "Exercise 2 result :\n" << TODO << "\n\n\n";
    }

    return 0;
}