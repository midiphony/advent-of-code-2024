#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <cmath>

#include <chrono>

int main(int argc, char *argv[])
{
    char *inputFileName;
    if (argc <= 2)
    {
        std::cout << "Provide input filename AND number of blinks" << std::endl;
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
    

    // Ultra brute force method
    std::vector<uint64_t> stones;
    uint64_t stone;

    // Parse data
    while (inFile >> stone)
    {
        stones.push_back(stone);
    }

    // DEBUG
    std::cout << "printing stones :\n";
    for (auto stone : stones)
    {
        std::cout << stone << " ";
    }
    std::cout << std::endl;

    // Exercise 1 
    {
        auto start = std::chrono::high_resolution_clock::now();

        while (blinksNb > 0)
        {
            for (int i = 0; i < stones.size(); i++)
            {
                uint64_t stoneValue = stones[i];
                if (stoneValue == 0)
                {
                    stones[i] = 1;
                    continue;
                }
                else if (int digitNb = log10(stoneValue) + 1; digitNb % 2 == 0)
                {
                    int middleDigitIndex = digitNb / 2;
                    int powerOfTenSplitValue = pow(10, middleDigitIndex);
                    int p1 = stoneValue / powerOfTenSplitValue;
                    int p2 = stoneValue % powerOfTenSplitValue;

                    auto posIter = stones.begin() + i;
                    stones[i] = p2;
                    stones.insert(posIter, p1);
                    i++;
                }
                else
                {
                    stones[i] *= 2024;
                }
            }

            blinksNb--;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // // DEBUG
        // for (auto stone : stones)
        // {
        //     std::cout << stone << " ";
        // }
        // std::cout << "\n\n\n";

        std::cout << "\n\n" << "Exercise 1 result :\n" << stones.size() << "\n\n\n";
        std::cout << "\n\n" << "Time to complete :\n" << (duration) << "us"<< "\n\n\n";
    }

    // Exercise 2
    {
        
        // std::cout << "\n\n" << "Exercise 2 result :\n" << TODO << "\n\n\n";
    }

    return 0;
}