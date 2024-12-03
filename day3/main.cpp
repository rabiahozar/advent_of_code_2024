#include <iostream>
#include <regex>
#include <string>
#include <fstream>

int main(int argc, char* argv[])
{
    bool mini_input = false;
    std::cout << argc << std::endl;
    if (argc >= 2) {
        std::cout << argv[1] << std::endl;
        if (std::string(argv[1]) == "mini") {
            mini_input = true;
        }
    }
    std::string fileName = mini_input ? "../mini_input" : "../input";
    std::ifstream infile(fileName);
    if (!infile) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return 1;
    }
    

    std::string input;
    std::regex re(R"((mul\(\d+,\d+\)))");
    std::regex reNumbers(R"(\d+,\d+)");
    std::string delimiter = ",";
    unsigned long long result = 0;

    while(std::getline(infile, input)) {

        std::sregex_iterator begin(input.begin(), input.end(), re);
        std::sregex_iterator end;
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            std::string multiplication = match.str();

            multiplication = multiplication.substr(4, multiplication.size() - 5);
            std::cout << "Match: " << multiplication << std::endl;

            auto commaPoint = multiplication.find(delimiter);
            if (commaPoint < multiplication.size()) {
                std::string firstNumber = multiplication.substr(0, commaPoint);
                std::string secondNumber = multiplication.substr(commaPoint + 1, multiplication.size());

                unsigned int first = std::stoi(firstNumber);
                unsigned int second = std::stoi(secondNumber);

                result += first * second;
                std::cout << "Result: " << result << std::endl;
            }
        }
    }

    std::cout << "Result: " << result << std::endl;
    return 0;
}