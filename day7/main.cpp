#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

bool g_verbose = false;

// Modify the calculation function to support the new || operator for
// concatenation
bool canCalculate(int64_t target, const std::vector<int64_t>& numbers,
                  size_t index = 1, int64_t current = 0)
{
    if (index == numbers.size())
    {
        return current == target;
    }

    if (canCalculate(target, numbers, index + 1, current + numbers[index]))
    {
        return true;
    }

    if (canCalculate(target, numbers, index + 1, current * numbers[index]))
    {
        return true;
    }

    std::stringstream ss;
    ss << current << numbers[index];
    int64_t concatenated;
    ss >> concatenated;
    if (canCalculate(target, numbers, index + 1, concatenated))
    {
        return true;
    }

    return false;
}

int main(int argc, char** argv)
{
    bool mini_input = false;

    if (argc >= 2 && std::string(argv[1]) == "mini")
    {
        mini_input = true;
    }
    if (argc >= 3 && std::string(argv[2]) == "verbose")
    {
        g_verbose = true;
    }

    std::string fileName = mini_input ? "../mini_input" : "../input";
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        std::cerr << "Could not open file" << std::endl;
        return 1;
    }

    std::string line;
    std::regex targetRegex(R"((\d+):)");
    std::vector<std::pair<int64_t, std::vector<int64_t>>> formulas;

    while (std::getline(file, line))
    {
        std::smatch match;
        if (!std::regex_search(line, match, targetRegex))
        {
            continue;
        }
        std::string targetStr = match.str(1);
        int64_t target = std::stoll(targetStr);

        line = line.substr(match.length());
        std::istringstream iss(line);
        std::vector<int64_t> numbers;
        int64_t num;
        while (iss >> num)
        {
            numbers.push_back(num);
        }

        formulas.push_back({target, numbers});
    }

    long long int sumOfCalibatedReports = 0;

    for (const auto& equation : formulas)
    {
        const auto& [target, numbers] = equation;

        if (numbers.empty())
        {
            continue;
        }

        // Start recursive calculation
        if (canCalculate(target, numbers, 1, numbers[0]))
        {
            sumOfCalibatedReports += target;
        }

        if (g_verbose)
        {
            std::cout << "Processed target: " << target << std::endl;
        }
    }

    std::cout << "Result part2: " << sumOfCalibatedReports << std::endl;
    return 0;
}
