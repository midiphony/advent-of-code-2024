#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

enum Cell
{
    Empty,
    Visited,
    Obstacle,
};

enum Direction
{
    Up,
    Right,
    Down,
    Left
};

struct Position
{
    int x, y;
};

struct Guard
{
    Position position;
    Direction direction;
};

using LabMap = std::vector<std::vector<Cell>>;

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

    LabMap labMap;
    Guard guard;
    Position mapParsingPosition = {0, 0};
    int rowSize = -1;

    // Parse map (upside-down for parsing : will need to reverse it afterwards)
    std::string currentLine;
    while (std::getline(inFile, currentLine))
    {
        std::vector<Cell> row;

        std::istringstream lineStream(currentLine);
        char cell;
        while(lineStream >> std::noskipws >> cell)
        {
            if (cell == '.')
            {
                row.push_back(Empty);
            }
            else if (cell == '\n')
            {
                break;
            }
            else if (cell == '#')
            {
                row.push_back(Obstacle);
            }
            else if (cell == '^')
            {
                row.push_back(Empty);
                guard.position = mapParsingPosition;
                guard.direction = Down;
            }
            else
            {
                std::cerr << "Unknown character in lab map input : " << cell << std::endl;
                return -1;
            }

            mapParsingPosition.x++;
        }

        if (rowSize >= 0 && rowSize != mapParsingPosition.x)
        {
            std::cerr << "Rows don't have the same size. Row of different size (upside-down) : " << mapParsingPosition.y << std::endl;
            return -2;
        }

        rowSize = mapParsingPosition.x;

        mapParsingPosition.x = 0;
        mapParsingPosition.y++;

        labMap.push_back(row);
    }

    // Flip map upside-down for convenience
    std::reverse(labMap.begin(), labMap.end());
    guard.position.y = labMap.size() - 1 - guard.position.y;
    guard.direction = Up;

    // // DEBUG
    // std::cout << "LabMap " << labMap.size() << std::endl;
    // for (LabMap::reverse_iterator riter = labMap.rbegin(); riter != labMap.rend(); ++riter)
    // {
    //     for(const Cell & cell : (*riter))
    //     {
    //         std::cout << cell;
    //     }

    //     std::cout << std::endl;
    // }
    // std::cout << "\n\n\n";

    // Exercise 1
    {
        int distinctVisitedPositionNumber = 0;

        Position &guardPosition = guard.position;
        while (guardPosition.x >= 0 && guardPosition.x < rowSize 
                && guardPosition.y >= 0 && guardPosition.y < labMap.size())
        {
            Position nextPosition = { -1, -1 };

            switch (guard.direction)
            {
                case Up:
                    nextPosition = {guardPosition.x, guardPosition.y + 1};
                    break;
                case Right:
                    nextPosition = {guardPosition.x + 1, guardPosition.y};
                    break;
                case Down:
                    nextPosition = {guardPosition.x, guardPosition.y - 1};
                    break;
                case Left:
                    nextPosition = {guardPosition.x - 1, guardPosition.y };
                    break;
                default:
                    std::cerr << "Unknown guard direction : " << guard.direction << std::endl;
                    return -3;
            }

            if (nextPosition.x >= 0 && nextPosition.x < rowSize 
                    && nextPosition.y >= 0 && nextPosition.y < labMap.size())
            {
                if (labMap[nextPosition.y][nextPosition.x] == Obstacle)
                {
                    // Assuming directions are ordered 90° right from each other
                    guard.direction = static_cast<Direction>((guard.direction + 1) % 4);
                    continue;
                }
            }

            if (labMap[guardPosition.y][guardPosition.x] == Empty)
            {
                labMap[guardPosition.y][guardPosition.x] = Visited;
                distinctVisitedPositionNumber++;
            }

            // // DEBUG :
            // for (int y = labMap.size() - 1; y >= 0; y--)
            // {
            //     for (int x = 0; x < rowSize; x++)
            //     {
            //         if (guardPosition.x == x && guardPosition.y == y)
            //             std::cout << 'X';
            //         else
            //             std::cout << labMap[y][x];
            //     }
            //     std::cout << std::endl;
            // }
            // std::cout << "\n\n\n";

            guardPosition = nextPosition;
        }

        std::cout << "\n\n" << "Exercise 1 result :\n" << distinctVisitedPositionNumber << "\n\n\n";
    }

    // Exercise 2
    {
        // int newValidUpdatesMiddlePageNumberSum = 0;

        // std::cout << "\n\n" << "Exercise 2 result :\n" << newValidUpdatesMiddlePageNumberSum << "\n\n\n";
    }

    return 0;
}