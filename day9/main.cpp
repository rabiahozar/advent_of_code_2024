#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool g_verbose = false;

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

    std::string diskmapRepresentation;
    std::getline(file, diskmapRepresentation);

    if (g_verbose)
        std::cout << diskmapRepresentation << std::endl;

    std::vector<int64_t> diskMap;
    int64_t id = 0;
    for (size_t i = 0; i < diskmapRepresentation.size(); i++)
    {
        int64_t element = i % 2 == 0 ? id++ : -1;

        for (int j = 0; j < int64_t(diskmapRepresentation[i] - '0'); j++)
        {
            diskMap.push_back(element);
        }
    }

    if (g_verbose)
    {
        for (auto item : diskMap)
        {
            if (item == -1)
            {
                std::cout << ".";
            }
            else
            {
                std::cout << item;
            }
        }
        std::cout << std::endl;
    }

    int lastMovableIndex = diskMap.size() - 1;
    for (size_t i = 0; i < diskMap.size(); i++)
    {
        if (diskMap[i] == -1)
        {
            while (lastMovableIndex > i && diskMap[lastMovableIndex] == -1)
            {
                lastMovableIndex--;
            }

            if (lastMovableIndex <= i)
                break;

            diskMap[i] = diskMap[lastMovableIndex];
            diskMap[lastMovableIndex] = -1;

            if (g_verbose)
            {
                for (auto item : diskMap)
                {
                    if (item == -1)
                        std::cout << ".";
                    else
                        std::cout << item;
                }
                std::cout << std::endl;
            }
        }
    }

    // Calculate the checksum
    long long int checksum = 0;
    for (size_t i = 0; i < diskMap.size(); i++)
    {
        if (diskMap[i] != -1)
        {
            checksum += i * diskMap[i];
        }
    }

    // Print final compacted disk state
    if (g_verbose)
    {
        for (auto item : diskMap)
        {
            if (item == -1)
                std::cout << ".";
            else
                std::cout << item;
        }
        std::cout << std::endl;
    }

    std::cout << "Result part 1: " << checksum << std::endl;
    return 0;
}
