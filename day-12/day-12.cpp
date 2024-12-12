#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <map>
#include <set>

#include <chrono>


// X and Y "Position" struct from day-06
struct Position
{
    int x, y;

    bool operator != (const Position& otherPosition) const
    {
        return x != otherPosition.x || y != otherPosition.y;
    }

    bool operator < (const Position& otherPosition) const
    {
        if (y < otherPosition.y)
            return true;
        else if (y > otherPosition.y)
            return false;
        
        return x < otherPosition.x;
    }
};

using Map = std::vector<std::vector<char>>;
using Region = std::vector<Position>;
using Regions = std::vector<Region>;
using RegionsPerType = std::map<char, Regions>;

bool IsAdjacent(const Position &posA, const Position &posB)
{
    int xDiff = abs(posA.x - posB.x);
    int yDiff = abs(posA.y - posB.y);

    return xDiff <= 1 && yDiff == 0 || xDiff == 0 && yDiff == 1;
}

bool IsAdjacentToRegion(const Position &pos, const Region &region)
{
    for(auto posFromRegion : region)
    {
        if (IsAdjacent(pos, posFromRegion))
            return true;
    }

    return false;
}

int GetNumberOfSides(const std::map<int, std::set<int>> & sideIndicesPerLine)
{
    int nbSides = 0;
    for (const auto &sideIndicesPerLineKvp : sideIndicesPerLine)
    {
        std::set<int> sideIndices = sideIndicesPerLineKvp.second;
        nbSides++;

        int previousIndex = *sideIndices.begin();

        for (std::set<int>::iterator sideIndicesIter = sideIndices.begin(); sideIndicesIter != sideIndices.end(); sideIndicesIter++)
        {
            int diff = abs((*sideIndicesIter) - previousIndex);
            if(diff > 1)
            {
                nbSides++;
            }

            previousIndex = *sideIndicesIter;
        }
    }

    return nbSides;
}

int main(int argc, char *argv[])
{
    char *inputFileName;
    if (argc < 2)
    {
        std::cout << "Provide input filename" << std::endl;
        return 1;
    }

    inputFileName = argv[1];

    std::ifstream inFile(inputFileName);
    if (!inFile.is_open())
    {
        std::cout << "failed to open " << inputFileName << '\n';
        return 2;
    }


    // Parse data
    Map map;
    Position mapParsingPosition = {0, 0};

    // Parse map (upside-down for parsing : will need to reverse it afterwards)
    std::string currentLine;
    while (!inFile.eof())
    {
        char character = inFile.get();
        if (character == '\n' || character == EOF)
            break;

        std::vector<char> row;
        while(!inFile.eof() && character != '\n')
        {
            row.push_back(character);
            character = inFile.get();
        }
        map.push_back(row);
    }

    // Flip map upside-down for convenience
    std::reverse(map.begin(), map.end());

    // // DEBUG
    // std::cout << "printing map :\n";
    // for (auto riter = map.rbegin(); riter != map.rend(); riter++)
    // {
    //     for (auto cell : *riter)
    //         std::cout << cell;

    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;

    RegionsPerType regionsPerType;

    // Exercise 1 
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (int y = 0; y < map.size(); y++)
        {
            int rowSize = map[y].size();
            for (int x = 0; x < rowSize; x++)
            {
                Position inspectedPosition = {x, y};
                Regions &regionsForCurrentType = regionsPerType[map[y][x]];

                std::set<int> adjacentRegionIndices;
                for (int regionIndex = 0; regionIndex < regionsForCurrentType.size(); regionIndex++)
                {
                    if (IsAdjacentToRegion(inspectedPosition, regionsForCurrentType[regionIndex]))
                        adjacentRegionIndices.insert(regionIndex);
                }

                if (adjacentRegionIndices.size() == 0)
                {
                    Region newRegion = {inspectedPosition};
                    regionsForCurrentType.push_back(newRegion);
                }
                else
                {
                    // Merge regions
                    Region &firstRegion = regionsForCurrentType[*adjacentRegionIndices.begin()];
                    for (auto otherAdjacentRegionIndexIter = adjacentRegionIndices.rbegin(); otherAdjacentRegionIndexIter != adjacentRegionIndices.rend(); otherAdjacentRegionIndexIter++)
                    {
                        Region &otherRegion = regionsForCurrentType[*otherAdjacentRegionIndexIter];

                        if (&firstRegion == &otherRegion) // TODO avoid to check this case via checking adjacentRegionIndices.rend()-1 : but I don't know why it doesn't compile
                            continue;

                        for (auto posFromOtherRegion : otherRegion)
                            firstRegion.push_back(posFromOtherRegion);

                        regionsForCurrentType.erase(regionsForCurrentType.begin() + *otherAdjacentRegionIndexIter);
                    }

                    firstRegion.push_back(inspectedPosition);
                }
            }
        }

        // // DEBUG
        // // PRINT REGION
        // for (auto regions : regionsPerType)
        // {
        //     std::cout << regions.first << " :\n";
            
        //     for (auto region : regions.second)
        //     {
        //         for (auto pos : region)
        //         {
        //             std::cout << '{' << pos.x << ',' << pos.y << '}';
        //         }
        //         std::cout << "\n";
        //     }

        //     std::cout << "\n";
        // }
        
        int totalPrice = 0;
        for (const auto &regionsForType : regionsPerType)
        {
            char parcelType = regionsForType.first;

            for (const auto &region : regionsForType.second)
            {
                // get area
                int area = region.size();
                int perimeter = 0;

                // Get perimeter
                for (const auto &parcelPosition : region)
                {
                    Position up = {parcelPosition.x, parcelPosition.y + 1};
                    Position down = {parcelPosition.x, parcelPosition.y - 1};
                    Position left = {parcelPosition.x-1, parcelPosition.y};
                    Position right = {parcelPosition.x+1, parcelPosition.y};
                    
                    if (up.y >= map.size() || map[up.y][up.x] != parcelType)
                        perimeter++;

                    if (down.y < 0 || map[down.y][down.x] != parcelType)
                        perimeter++;

                    if (left.x < 0 || map[left.y][left.x] != parcelType)
                        perimeter++;

                    if (right.x >= map[parcelPosition.y].size() || map[right.y][right.x] != parcelType)
                        perimeter++;
                }

                // // DEBUG
                // std::cout << parcelType << " : " << area << " * " << perimeter << " = " << area * perimeter << std::endl;

                totalPrice += area * perimeter;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "\n\n" << "Exercise 1 result :\n" << totalPrice << "\n\n";
        std::cout << "Time to complete :\n" << (duration) << "us"<< "\n\n\n";
    }

    // Exercise 2
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        int totalPrice = 0;
        for (const auto &regionsForType : regionsPerType)
        {
            char parcelType = regionsForType.first;

            for (const auto &region : regionsForType.second)
            {
                // get area
                int area = region.size();

                Position bottomLeftRegionPosition = region[0]; // most bottom has priority to most left
                for (const auto &parcelPosition : region)
                {
                    if (parcelPosition.y < bottomLeftRegionPosition.y)
                        bottomLeftRegionPosition = parcelPosition;
                    else if (parcelPosition.y == bottomLeftRegionPosition.y && parcelPosition.x < bottomLeftRegionPosition.x)
                    {
                        bottomLeftRegionPosition = parcelPosition;
                    }
                }

                std::map<int, std::set<int>> leftSideYIndicesPerXIndex;
                std::map<int, std::set<int>> rightSideYIndicesPerXIndex;
                std::map<int, std::set<int>> upSideXIndicesPerYIndex;
                std::map<int, std::set<int>> downSideXIndicesPerYindex;

                // Get perimeter
                for (const auto &parcelPosition : region)
                {
                    Position up = {parcelPosition.x, parcelPosition.y + 1};
                    Position down = {parcelPosition.x, parcelPosition.y - 1};
                    Position left = {parcelPosition.x-1, parcelPosition.y};
                    Position right = {parcelPosition.x+1, parcelPosition.y};
                    
                    if (up.y >= map.size() || map[up.y][up.x] != parcelType)
                        upSideXIndicesPerYIndex[up.y].insert(up.x);

                    if (down.y < 0 || map[down.y][down.x] != parcelType)
                        downSideXIndicesPerYindex[down.y].insert(down.x);

                    if (left.x < 0 || map[left.y][left.x] != parcelType)
                        leftSideYIndicesPerXIndex[left.x].insert(left.y);

                    if (right.x >= map[parcelPosition.y].size() || map[right.y][right.x] != parcelType)
                        rightSideYIndicesPerXIndex[right.x].insert(right.y);
                }

                int nbSides = 0;
                nbSides += GetNumberOfSides(upSideXIndicesPerYIndex);
                nbSides += GetNumberOfSides(downSideXIndicesPerYindex);
                nbSides += GetNumberOfSides(leftSideYIndicesPerXIndex);
                nbSides += GetNumberOfSides(rightSideYIndicesPerXIndex);

                // // DEBUG
                // std::cout << parcelType << " : " << area << " * " << nbSides << " = " << area * nbSides << std::endl;

                totalPrice += area * nbSides;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "\n\n" << "Exercise 2 result :\n" << totalPrice << "\n\n";
        std::cout << "Time to complete :\n" << (duration) << "us"<< "\n\n\n";
    }

    return 0;
}