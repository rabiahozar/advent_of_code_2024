#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

bool g_verbose = false;
bool hasAntenna(const std::vector<std::vector<char>>& map, size_t x, size_t y,
                char type)
{
    if (x >= 0 && y >= 0 && x < map.size() && !map.empty() &&
        y < map[0].size() && map[x][y] == type)
        return true;

    return false;
}

bool addAntinode(const std::vector<std::vector<char>>& map, int64_t x,
                 int64_t y, int64_t distanceX, int64_t distanceY,
                 std::set<std::pair<int64_t, int64_t>>& antinodes)
{
    if (x - distanceX >= 0 && x - distanceX < map.size() &&
        y - distanceY >= 0 && y - distanceY < map[0].size())
    {
        if (g_verbose)
            std::cout << "Adding antinode [" << x - distanceX << ", "
                      << y - distanceY << "]" << std::endl;
        antinodes.emplace(x - distanceX, y - distanceY);
        return true;
    }
    return false;
}

void findAntinodes(const std::vector<std::vector<char>>& map)
{
    std::set<std::pair<int64_t, int64_t>> antinodes;
    for (size_t x = 0; x < map.size(); x++)
    {
        for (size_t y = 0; y < map[0].size(); y++)
        {
            if (map[x][y] != '.')
            {
                if (g_verbose)
                    std::cout << "Antenna  [" << x << " , " << y
                              << " ] : " << map[x][y] << std::endl;
                for (size_t i = x + 1; i < map.size(); i++)
                {
                    for (size_t j = 0; j < map[0].size(); j++)
                    {
                        if (hasAntenna(map, i, j, map[x][y]))
                        {
                            if (g_verbose)
                                std::cout << "Another Antenna  [" << i << " , "
                                          << j << " ]" << std::endl;
                            int distanceX = x - i;  // -1
                            int distanceY = y - j;  // 3
                            addAntinode(map, x, y, distanceX * (-1),
                                        distanceY * (-1), antinodes);
                            addAntinode(map, i, j, distanceX, distanceY,
                                        antinodes);
                        }
                    }
                }
            }
        }
    }

    std::cout << "Result 1: " << antinodes.size() << std::endl;
}

void findAntinodesWithCalibration(const std::vector<std::vector<char>>& map)
{
    std::set<std::pair<int64_t, int64_t>> antinodes;
    for (size_t x = 0; x < map.size(); x++)
    {
        for (size_t y = 0; y < map[0].size(); y++)
        {
            if (map[x][y] != '.')
            {
                std::cout << "Antenna  [" << x << " , " << y
                          << " ] : " << map[x][y] << std::endl;
                for (size_t i = x + 1; i < map.size(); i++)
                {
                    for (size_t j = 0; j < map[0].size(); j++)
                    {
                        if (hasAntenna(map, i, j, map[x][y]))
                        {
                            std::cout << "Another Antenna  [" << i << " , " << j
                                      << " ]" << std::endl;
                            int distanceX = x - i;  // -1
                            int distanceY = y - j;  // 3

                            int64_t referenceX = x;
                            int64_t referenceY = y;
                            while (addAntinode(map, referenceX, referenceY,
                                               distanceX * (-1),
                                               distanceY * (-1), antinodes))
                            {
                                referenceX -= distanceX * (-1);
                                referenceY -= distanceY * (-1);
                                                        }

                            referenceX = i;
                            referenceY = j;
                            while (addAntinode(map, referenceX, referenceY,
                                               distanceX, distanceY, antinodes))
                            {
                                referenceX -= distanceX;
                                referenceY -= distanceY;
                            }
                            antinodes.emplace(x, y);
                            antinodes.emplace(i, j);
                        }
                    }
                }
            }
        }
    }

    std::cout << "Result 2: " << antinodes.size() << std::endl;
}

int main(int argc, char** argv)
{
    bool mini_input = false;

    if (argc >= 2 && std::string(argv[1]) == "mini")
    {
        mini_input = true;
    }
    bool verbose = false;
    if (argc >= 3 && std::string(argv[2]) == "verbose")
    {
        verbose = true;
        g_verbose = verbose;
    }

    std::string fileName = mini_input ? "../mini_input" : "../input";
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        std::cerr << "Could not open file" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::vector<char>> map;
    if (verbose)
        std::cout << 0;
    while (std::getline(file, line))
    {
        if (verbose)
        {
            if (map.empty())
            {
                for (size_t i = 0; i < line.size(); i++)
                {
                    std::cout << i % 10;
                }
                std::cout << std::endl;
            }
            std::cout << (map.size()) % 10;
            std::cout << line << std::endl;
        }
        map.push_back({line.begin(), line.end()});
    }

    // findAntinodes(map);
    findAntinodesWithCalibration(map);

    return 0;
}
