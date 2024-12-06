#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>

constexpr char OBSTACLE = '#';
constexpr char GUARD = '^';
constexpr char VISITED = 'X';

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

void moveAlong(size_t& x, size_t& y, Direction& direction)
{
    switch (direction)
    {
        case UP:
            x--;
            break;
        case DOWN:
            x++;
            break;
        case LEFT:
            y--;
            break;
        case RIGHT:
            y++;
            break;
        default:
            break;
    }
}

void turnRight(size_t& x, size_t& y, Direction& direction)
{
    switch (direction)
    {
        case UP:
            direction = RIGHT;
            x++;
            break;
        case DOWN:
            direction = LEFT;
            x--;
            break;
        case LEFT:
            direction = UP;
            y++;
            break;
        case RIGHT:
            direction = DOWN;
            y--;
            break;
    }
}

int markUntilObstacle(std::vector<std::vector<char>>& input, size_t& x,
                      size_t& y, Direction direction)
{
    int count = 0;
    while (x < input.size() && x >= 0 && y < input[0].size() && y >= 0 &&
           input[x][y] != OBSTACLE)
    {
        if (input[x][y] != VISITED)
        {
            input[x][y] = VISITED;
            count++;
        }

        moveAlong(x, y, direction);
    }

    return count;
}

int markAndCountVisitedCoordinates(std::vector<std::vector<char>>& input,
                                   size_t x, size_t y)
{
    Direction direction = UP;
    int count = 0;
    while (true)
    {
        count += markUntilObstacle(input, x, y, direction);
        // std::cout << "Guard returned at x: "<< startingX << ", y: " <<
        // startingY << " direction:"<< direction<< std::endl;
        turnRight(x, y, direction);
        if (x == input.size() - 1 || x < 0 || y == input[0].size() - 1 || y < 0)
        {
            break;
        }
    }
    return count;
}

bool detectLoop(const std::vector<std::vector<char>>& input, size_t x, size_t y)
{
    Direction direction = UP;
    std::set<std::tuple<size_t, size_t, Direction>>
        visitedCell;  // Track cell and direction
    while (true)
    {
        if (input[x][y] == OBSTACLE)
        {
            turnRight(x, y, direction);
        }
        else
        {
            auto cell = std::make_tuple(x, y, direction);
            if (visitedCell.count(cell) > 0)
            {
                // std::cout << "Detected a loop at (" << x << ", " << y << ")"
                //           << std::endl;
                return true;
            }
            visitedCell.insert(cell);

            moveAlong(x, y, direction);
        }

        if (x >= input.size() || x < 0 || y >= input[0].size() || y < 0)
        {
            break;
        }
    }
    return false;
}

int main(int argc, char** argv)
{
    bool mini_input = false;

    if (argc >= 2)
    {
        if (std::string(argv[1]) == "mini")
        {
            mini_input = true;
        }
    }
    bool verbose = false;
    if (argc >= 3)
    {
        if (std::string(argv[2]) == "verbose")
        {
            verbose = true;
        }
    }

    std::string fileName = mini_input ? "../mini_input" : "../input";
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        std::cerr << "Could not open file" << std::endl;
        return 1;
    }

    std::vector<std::vector<char>> input;
    std::string line;
    size_t _startingX = 0, _startingY = 0;

    while (std::getline(file, line))
    {
        input.push_back({line.begin(), line.end()});
        auto guardPosY = line.find(GUARD);
        if (guardPosY != std::string::npos)
        {
            _startingX = input.size() - 1;
            _startingY = guardPosY;
        }
    }
    std::cout << "Guard found at (" << _startingX << ", " << _startingY << ")"
              << std::endl;

    int count = markAndCountVisitedCoordinates(input, _startingX, _startingY);

    if (verbose)
    {
        for (int i = 0; i < input.size(); ++i)
        {
            for (int j = 0; j < input[i].size(); ++j)
            {
                std::cout << input[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << "Result part1: " << count << std::endl;

    int possibleObstaclePosition = 0;
    for (size_t i = 0; i < input.size(); i++)
    {
        for (size_t j = 0; j < input[0].size(); j++)
        {
            if (input[i][j] == VISITED)
            {
                input[i][j] = OBSTACLE;
                if (detectLoop(input, _startingX, _startingY))
                {
                    possibleObstaclePosition++;
                }
                input[i][j] = VISITED;
            }
        }
    }
    std::cout << "Result part2: " << possibleObstaclePosition << std::endl;

    return 0;
}