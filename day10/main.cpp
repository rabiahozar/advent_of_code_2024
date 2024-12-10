#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

bool g_verbose = false;

void traverse(std::vector<std::vector<int64_t>>& map,
              std::map<std::pair<size_t, size_t>, int64_t>& rating, int64_t x,
              int64_t y)
{
    if (map[x][y] == 9)
    {
        auto it = rating.find({x, y});
        if (it != rating.end())
        {
            it->second++;
        }
        else
        {
            rating.emplace(std::make_pair<size_t, size_t>(x, y), 1);
        }
        return;
    }

    // try to x+1 y+1 x-1 y-1
    if (x + 1 < map.size() && map[x + 1][y] == map[x][y] + 1)
    {
        traverse(map, rating, x + 1, y);
    }

    if (x - 1 >= 0 && map[x - 1][y] == map[x][y] + 1)
    {
        traverse(map, rating, x - 1, y);
    }

    if (y + 1 < map[0].size() && map[x][y + 1] == map[x][y] + 1)
    {
        traverse(map, rating, x, y + 1);
    }

    if (y - 1 >= 0 && map[x][y - 1] == map[x][y] + 1)
    {
        traverse(map, rating, x, y - 1);
    }

    // maybe add visited?? -> not needed only can go higher
}

int main(int argc, char** argv)
{
    bool mini_input = argc >= 2 && std::string(argv[1]) == "mini";
    g_verbose = argc >= 3 && std::string(argv[2]) == "verbose";

    std::string fileName = mini_input ? "../mini_input" : "../input";
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        std::cerr << "Could not open file" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::vector<int64_t>> map{};
    while (std::getline(file, line))
    {
        map.push_back({});
        for (auto ch : line)
        {
            map.back().push_back(ch - '0');
        }
    }

    int64_t score{0};
    int64_t totalRating{0};
    // find 0 in map and traverse
    for (size_t i = 0; i < map.size(); i++)
    {
        for (size_t j = 0; j < map[0].size(); j++)
        {
            if (map[i][j] == 0)
            {
                std::map<std::pair<size_t, size_t>, int64_t> ratings{};
                traverse(map, ratings, i, j);
                score += ratings.size();
                for (const auto& rating : ratings)
                {
                    totalRating += rating.second;
                }
            }
        }
    }

    if (g_verbose)
    {
        for (const auto& m : map)
        {
            for (auto item : m)
            {
                std::cout << item;
            }
            std::cout << std::endl;
        }
    }

    // number of unique 9s that has been reached from 9

    std::cout << "Result part 1: " << score << std::endl;
    std::cout << "Result part 2: " << totalRating << std::endl;

    return 0;
}
