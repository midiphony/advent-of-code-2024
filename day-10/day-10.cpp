#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <set>

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

using Map = std::vector<std::vector<int>>;

void FindDestinationsFrom(const Position &currentCellPosition, const Map &map, std::set<Position> &uniqueDestinations, int &totalTrailsNumber)
{
    int currentCellValue = map[currentCellPosition.y][currentCellPosition.x];
    if (currentCellValue >= 9)
    {
        totalTrailsNumber++;
        uniqueDestinations.insert(currentCellPosition);
        return;
    }

    int nextExpectedCellValue = currentCellValue + 1;

    Position up = {currentCellPosition.x, currentCellPosition.y + 1};
    Position down = {currentCellPosition.x, currentCellPosition.y - 1};
    Position left = {currentCellPosition.x-1, currentCellPosition.y};
    Position right = {currentCellPosition.x+1, currentCellPosition.y};

    if(up.y < map.size())
    {
        if (map[up.y][up.x] == nextExpectedCellValue)
            FindDestinationsFrom(up, map, uniqueDestinations, totalTrailsNumber);
    }

    if (down.y >= 0)
    {
        if (map[down.y][down.x] == nextExpectedCellValue)
            FindDestinationsFrom(down, map, uniqueDestinations, totalTrailsNumber);
    }

    if (left.x >= 0)
    {
        if (map[left.y][left.x] == nextExpectedCellValue)
            FindDestinationsFrom(left, map, uniqueDestinations, totalTrailsNumber);
    }

    if (right.x < map[currentCellPosition.x].size())
    {
        if (map[right.y][right.x] == nextExpectedCellValue)
            FindDestinationsFrom(right, map, uniqueDestinations, totalTrailsNumber);
    }

    return;
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

    std::vector<int> diskMap;

    // Parse data

    Map map;
    Position mapParsingPosition = {0, 0};
    int rowSize = -1;

    // Parse map (upside-down for parsing : will need to reverse it afterwards)
    std::string currentLine;
    while (!inFile.eof())
    {
        char character = inFile.get();
        if (character == '\n' || character == EOF)
            break;

        std::vector<int> row;
        while(!inFile.eof() && character != '\n')
        {
            row.push_back(std::atoi(&character));
            character = inFile.get();
        }
        map.push_back(row);
    }

    // Flip map upside-down for convenience
    std::reverse(map.begin(), map.end());

    // DEBUG
    std::cout << "printing map :\n";
    for (auto riter = map.rbegin(); riter != map.rend(); riter++)
    {
        for (auto cell : *riter)
            std::cout << cell;

        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Exercise 1 and 2 together (we can keep the number of trailheads AND the number of unique trails at the same time)
    {
        int validTrailheadsNumber = 0;
        int uniqueTrailsNumber = 0;

        std::set<Position> uniqueDestinationsFromCurrentCell;

        for (int y = 0; y < map.size(); y++)
        {
            for (int x = 0; x < map[y].size(); x++)
            {
                if (map[y][x] != 0)
                    continue;

                int totalTrailsNumber = 0;

                FindDestinationsFrom({x, y}, map, uniqueDestinationsFromCurrentCell, uniqueTrailsNumber);

                validTrailheadsNumber += uniqueDestinationsFromCurrentCell.size();
                uniqueDestinationsFromCurrentCell.clear();
            }
        }

        std::cout << "\n\n" << "Exercise 1 result :\n" << validTrailheadsNumber << "\n\n\n";
        std::cout << "\n\n" << "Exercise 2 result :\n" << uniqueTrailsNumber << "\n\n\n";
    }

    return 0;
}