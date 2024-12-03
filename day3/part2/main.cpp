#include <iostream>
#include <regex>
#include <string>
#include <fstream>

unsigned long long processMultiplications(const std::string& input, const std::regex& re, const std::string& delimiter) {
    unsigned long long result = 0;

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
            std::string secondNumber = multiplication.substr(commaPoint + 1);

            unsigned int first = std::stoi(firstNumber);
            unsigned int second = std::stoi(secondNumber);

            result += first * second;
        }
    }

    return result;
}

int main(int argc, char* argv[]) {
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

    std::string input = "";
    std::string line;
    while (std::getline(infile, line)) {
        input += line;
    }

    std::cout << input << std::endl;

    const std::regex re(R"((mul\(\d+,\d+\)))");
    const std::string delimiter = ",";

    unsigned long long result = 0;
    std::string doString = "do()";

    while (true) {
        size_t dontPos = input.find("don't()");
        if (dontPos == std::string::npos) {
            result += processMultiplications(input, re, delimiter);  
            break;
        }

        std::string toCheck = input.substr(0, dontPos);
        std::string rest = input.substr(dontPos, input.size());

        result += processMultiplications(toCheck, re, delimiter); 

        size_t doPos = rest.find(doString);
        if (doPos == std::string::npos) {
            break;
        }

        input = rest.substr(doPos + doString.size(), rest.size());
    }

    std::cout << "Result: " << result << std::endl;
    return 0;
}
