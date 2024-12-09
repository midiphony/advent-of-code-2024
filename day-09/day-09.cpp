#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

const int FREE_SPACE = -1;

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

    std::vector<int> diskMap;

    // Parse data
    char nextChar = inFile.get();
    int currentFileId = 0;
    bool isEvaluatingFreeSpace = false;
    while (inFile.eof() == false && nextChar != '\n')
    {
        int spaceLength = std::atoi(&nextChar);
        if(!isEvaluatingFreeSpace)
        {
            if (spaceLength <= 0)
            {
                std::cerr << "Found a file with space size <= 0, at currentFileId : " << currentFileId << std::endl;
                return -1;
            }

            int remainingSpaceToFillWithFileId = spaceLength;
            for (; remainingSpaceToFillWithFileId > 0; remainingSpaceToFillWithFileId--)
            {
                diskMap.push_back(currentFileId);
            }

            currentFileId++;
        }
        else
        {
            int remainingSpaceToFillWithFreeSpace = spaceLength;
            for (; remainingSpaceToFillWithFreeSpace > 0; remainingSpaceToFillWithFreeSpace--)
            {
                diskMap.push_back(FREE_SPACE);
            }
        }

        isEvaluatingFreeSpace = !isEvaluatingFreeSpace;
        nextChar = inFile.get();
    }

    // // DEBUG
    // std::cout << "printing disk map :\n";
    // for (auto diskCell : diskMap)
    // {
    //     if (diskCell == FREE_SPACE)
    //         std::cout << '.';
    //     else
    //         std::cout << diskCell;
    // }
    // std::cout << std::endl;

    // Exercise 1
    {
        std::vector<int> diskMapPartOne(diskMap);

        std::vector<int>::iterator freeSpaceCursor = diskMapPartOne.begin();
        std::vector<int>::iterator fileCursor = diskMapPartOne.end() - 1;

        while (freeSpaceCursor < fileCursor)
        {
            while (*freeSpaceCursor != FREE_SPACE)
                freeSpaceCursor++;
            
            while(*fileCursor == FREE_SPACE)
                fileCursor--;
            
            if (fileCursor <= freeSpaceCursor)
                break;

            int temp = *freeSpaceCursor;
            *freeSpaceCursor = *fileCursor;
            *fileCursor = temp;
        }

        // // DEBUG
        // std::cout << "New disk map : \n\n";
        // for (auto diskCell : diskMap)
        // {
        //     if (diskCell == FREE_SPACE)
        //         std::cout << '.';
        //     else
        //         std::cout << diskCell;
        // }

        uint64_t checkSum = 0;
        for (int i = 0; i < diskMapPartOne.size() && diskMapPartOne[i] != FREE_SPACE; i++)
        {
            // // DEBUG
            // std::cout << i * diskMapPartOne[i] << std::endl;
            checkSum += i * diskMapPartOne[i];
        }

        std::cout << "\n\n" << "Exercise 1 result :\n" << checkSum << "\n\n\n";
    }

    // Exercise 2
    // // TODO : store array of block segments (with value, space, maybe index) to avoid recomputing free-space everytime
    {
        std::vector<int> diskMapPartTwo(diskMap);

        std::vector<int>::iterator fileCursor = diskMapPartTwo.end() - 1;
        int currentFileId = *fileCursor;

        // Dumb way test
        while (fileCursor != diskMapPartTwo.begin() || currentFileId < 0)
        {
            while (fileCursor != diskMapPartTwo.begin() && currentFileId != *fileCursor)
            {
                fileCursor--;
            }

            if (fileCursor == diskMapPartTwo.begin())
            {
                // end of file id movement from right to left
                break;
            }

            int neededSpace = 0;
            while(fileCursor != diskMapPartTwo.begin() && currentFileId == *fileCursor)
            {
                neededSpace++;
                fileCursor--;
            }

            if (fileCursor == diskMapPartTwo.begin())
            {
                // end of file id movement from right to left
                break;
            }

            std::vector<int>::iterator freeSpaceCursorBeginningOfSpace = diskMapPartTwo.begin();
            bool foundSpace = false;

            while (freeSpaceCursorBeginningOfSpace < fileCursor && !foundSpace)
            {
                if (*freeSpaceCursorBeginningOfSpace == FREE_SPACE)
                {
                    std::vector<int>::iterator freeSpaceCursorEndOfSpace = freeSpaceCursorBeginningOfSpace;
                    int availableSpace = 0;
                    for (availableSpace = 0; availableSpace < neededSpace; availableSpace++)
                    {
                        if (*freeSpaceCursorEndOfSpace != FREE_SPACE)
                            break;

                        freeSpaceCursorEndOfSpace++;
                    }

                    if (availableSpace == neededSpace)
                    {
                        foundSpace = true;

                        while (freeSpaceCursorBeginningOfSpace != freeSpaceCursorEndOfSpace)
                        {
                            *freeSpaceCursorBeginningOfSpace = currentFileId;
                            freeSpaceCursorBeginningOfSpace++;
                        }
                    }
                    else
                    {
                        freeSpaceCursorBeginningOfSpace = freeSpaceCursorEndOfSpace;
                    }
                }
                else
                {
                    freeSpaceCursorBeginningOfSpace++;
                }
            }

            if (foundSpace)
            {
                std::vector<int>::iterator fileToRemoveIterator(fileCursor+1);
                while (neededSpace > 0)
                {
                    *fileToRemoveIterator = FREE_SPACE;
                    fileToRemoveIterator++;
                    neededSpace--;
                }
            }

            currentFileId--;
        }

        // // DEBUG
        // std::cout << "New disk map : \n\n";
        // for (auto diskCell : diskMapPartTwo)
        // {
        //     if (diskCell == FREE_SPACE)
        //         std::cout << '.';
        //     else
        //         std::cout << diskCell;
        // }
        // std::cout << "\n\n";

        uint64_t checkSum = 0;
        for (int i = 0; i < diskMapPartTwo.size(); i++)
        {
            if (diskMapPartTwo[i] == FREE_SPACE)
                continue;

            // // DEBUG 
            // std::cout << i << "*" << diskMapPartTwo[i] << "\t=" << i*diskMapPartTwo[i] << std::endl;
            checkSum += i * diskMapPartTwo[i];
        }

        std::cout << "\n\n" << "Exercise 2 result :\n" << checkSum << "\n\n\n";
    }

    return 0;
}