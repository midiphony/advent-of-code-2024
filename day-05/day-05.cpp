#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <map>
#include <set>
#include <iterator>

// #include <string>

bool IsUpdateInvalid(const std::vector<int> & update, const std::map<int, std::set<int>> & priorityRules, 
                        int startingCheckIndex, int & problematicPriorNumberIndex, int & problematicLaterNumberIndex)
{
    bool isUpdateInvalid = false;
    int checkIndex = startingCheckIndex;

    for (; checkIndex < update.size() - 1; checkIndex++)
    {
        int firstNumber = update[checkIndex];

        std::vector<int>::const_iterator constUpdateIt = update.begin();

        for (std::vector<int>::const_iterator laterNumberIterator = update.cbegin() + checkIndex + 1; laterNumberIterator != update.end() && false == isUpdateInvalid; laterNumberIterator++)
        {
            int laterNumber = *laterNumberIterator;

            std::map<int, std::set<int>>::const_iterator kvpFromPriorityRule = priorityRules.find(laterNumber);
            if (kvpFromPriorityRule != priorityRules.end() && kvpFromPriorityRule->second.find(firstNumber) != kvpFromPriorityRule->second.end())
            {
                isUpdateInvalid = true;

                // // DEBUG :
                // std::cout << "Following update is invalid : ";
                // for (int updateNumber : update)
                //     std::cout << updateNumber << ",";

                // std::cout << "\t because of numbers : " << laterNumber << "|" << firstNumber << std::endl;

                problematicPriorNumberIndex = checkIndex;
                problematicLaterNumberIndex = std::distance(update.cbegin(), laterNumberIterator);
            }
        }
    }

    return isUpdateInvalid;
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
    

    std::map<int, std::set<int>> priorityRules;
    std::vector<std::vector<int>> updatesList;
    std::vector<std::vector<int>> incorrectUpdatesList; // For part two

    // Exercise 1
    {
        int validUpdatesMiddlePageNumberSum = 0;

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
        while (inFile.peek() != EOF)
        {
            int newNumber;
            std::vector<int> update;
            while (inFile >> newNumber && inFile.get() == ',')
                update.push_back(newNumber);

            update.push_back(newNumber);

            updatesList.push_back(update);
        }

        for(auto & update : updatesList)
        {
            bool isUpdateInvalid = false;

            int problematicPriorNumberIndex, problematicLaterNumberIndex;
            if (false == IsUpdateInvalid(update, priorityRules, 0, problematicPriorNumberIndex, problematicLaterNumberIndex))
            {
                int middleValue = *(update.begin() + update.size()/2);

                // // DEBUG
                // std::cout << "Valid : ";
                // for (int updateNumber : update)
                //     std::cout << updateNumber << ",";
                // std::cout << "\t middle : " << middleValue << std::endl;

                validUpdatesMiddlePageNumberSum += middleValue;
            }
            else
            {
                incorrectUpdatesList.push_back(update);
            }
        }

        std::cout << "\n\n" << "Exercise 1 result :\n" << validUpdatesMiddlePageNumberSum << "\n\n\n";
    }

    // Exercise 2
    {
        int newValidUpdatesMiddlePageNumberSum = 0;

        for(std::vector<int> updateToFix : incorrectUpdatesList)
        {
            int checkIndex = 0;
            bool updateStillInvalid = true;

            while (checkIndex < updateToFix.size() && true == updateStillInvalid)
            {
                int problematicPriorNumberIndex, problematicLaterNumberIndex;
                if (IsUpdateInvalid(updateToFix, priorityRules, checkIndex, problematicPriorNumberIndex, problematicLaterNumberIndex))
                {
                    checkIndex = problematicPriorNumberIndex;

                    // swap
                    int tmp = updateToFix[problematicLaterNumberIndex];
                    updateToFix[problematicLaterNumberIndex] = updateToFix[problematicPriorNumberIndex];
                    updateToFix[problematicPriorNumberIndex] = tmp;
                }
                else
                {
                    updateStillInvalid = false;
                }
            }

            int middleValue = *(updateToFix.begin() + updateToFix.size()/2);

            // // DEBUG
            // std::cout << "Fixed : ";
            // for (int updateNumber : updateToFix)
            //     std::cout << updateNumber << ",";
            // std::cout << "\t middle : " << middleValue << std::endl;

            newValidUpdatesMiddlePageNumberSum += middleValue;
        }
        

        std::cout << "\n\n" << "Exercise 2 result :\n" << newValidUpdatesMiddlePageNumberSum << "\n\n\n";
    }

    return 0;
}