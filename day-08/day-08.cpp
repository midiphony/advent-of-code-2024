#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <map>
#include <set>

using Position = std::pair<int, int>;
using AntennaFrequency = char;
using PositionsByAntennaFrequency = std::map<AntennaFrequency, std::vector<Position>>;

template <typename T,typename U>                                                   
std::pair<T,U> operator+(const std::pair<T,U> & l,const std::pair<T,U> & r) {   
    return {l.first+r.first,l.second+r.second};                                    
}     

template <typename T,typename U>                                                   
std::pair<T,U> operator-(const std::pair<T,U> & l,const std::pair<T,U> & r) {   
    return {l.first-r.first,l.second-r.second};                                    
}

bool IsPositionWithinMap(const Position & position, const std::pair<int,int> & mapMaxSize)
{
    return (position.first >= 0 && position.first < mapMaxSize.first 
            && position.second >= 0 && position.second < mapMaxSize.second);
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

    PositionsByAntennaFrequency positionsByAntennaFrequency;

    // Parse map (upside-down for parsing : will need to reverse it afterwards)
    std::string currentLine;
    int x = 0, y = 0;
    while (std::getline(inFile, currentLine))
    {
        std::istringstream lineStream(currentLine);

        x = 0;

        char potentialAntenna;
        while (lineStream >> potentialAntenna)
        {
            if (potentialAntenna != '.')
            {
                positionsByAntennaFrequency[potentialAntenna].push_back({x, y});
            }
            x++;
        }
        y++;
    }

    std::pair<int, int> mapMaxSize(x, y);

    // DEBUG
    std::cout << "printing antenna positions : \n\n";
    for (auto antennaPositions : positionsByAntennaFrequency)
    {
        std::cout << "Antenna " << antennaPositions.first << " : ";
        for (auto position : antennaPositions.second)
        {
            std::cout << "{\t" << position.first << ",\t" << position.second << "} ;";
        }
        std::cout << std::endl;
    }

    // Exercise 1
    {
        std::set<Position> uniqueAntinodePositions;
        for (auto antennaPositions : positionsByAntennaFrequency)
        {
            for (std::vector<Position>::iterator positionIterator = antennaPositions.second.begin(); positionIterator != antennaPositions.second.end() - 1; positionIterator++)
            {
                Position firstAntennaPosition = *positionIterator;

                for (std::vector<Position>::iterator nextPositionIterator = positionIterator+1; nextPositionIterator != antennaPositions.second.end(); nextPositionIterator++)
                {
                    Position secondAntennaPosition = *nextPositionIterator;

                    Position antennaVector = secondAntennaPosition - firstAntennaPosition;

                    Position firstPotentialAntinodePosition = firstAntennaPosition - antennaVector;
                    Position secondPotentialAntinodePosition = secondAntennaPosition + antennaVector;

                    if (IsPositionWithinMap(firstPotentialAntinodePosition, mapMaxSize))
                        uniqueAntinodePositions.insert(firstPotentialAntinodePosition);
                    
                    if (IsPositionWithinMap(secondPotentialAntinodePosition, mapMaxSize))
                        uniqueAntinodePositions.insert(secondPotentialAntinodePosition);
                }
            }
        }

        std::cout << "\n\n" << "Exercise 1 result :\n" << uniqueAntinodePositions.size() << "\n\n\n";
    }

    // Exercise 2
    {
        std::set<Position> uniqueAntinodePositions;
        for (auto antennaPositions : positionsByAntennaFrequency)
        {
            for (std::vector<Position>::iterator positionIterator = antennaPositions.second.begin(); positionIterator != antennaPositions.second.end() - 1; positionIterator++)
            {
                Position firstAntennaPosition = *positionIterator;
                uniqueAntinodePositions.insert(firstAntennaPosition);

                for (std::vector<Position>::iterator nextPositionIterator = positionIterator+1; nextPositionIterator != antennaPositions.second.end(); nextPositionIterator++)
                {
                    Position secondAntennaPosition = *nextPositionIterator;

                    Position antennaVector = secondAntennaPosition - firstAntennaPosition;

                    Position firstPotentialAntinodePosition = firstAntennaPosition - antennaVector;
                    Position secondPotentialAntinodePosition = secondAntennaPosition + antennaVector;

                    while(IsPositionWithinMap(firstPotentialAntinodePosition, mapMaxSize))
                    {
                        uniqueAntinodePositions.insert(firstPotentialAntinodePosition);
                        firstPotentialAntinodePosition = firstPotentialAntinodePosition - antennaVector;
                    }
                    
                    while (IsPositionWithinMap(secondPotentialAntinodePosition, mapMaxSize))
                    {
                        uniqueAntinodePositions.insert(secondPotentialAntinodePosition);
                        secondPotentialAntinodePosition = secondPotentialAntinodePosition + antennaVector;
                    }
                }
            }

            if (antennaPositions.second.size() > 1)
                uniqueAntinodePositions.insert(antennaPositions.second.back());
        }

        std::cout << "\n\n" << "Exercise 2 result :\n" << uniqueAntinodePositions.size() << "\n\n\n";
    }

    return 0;
}