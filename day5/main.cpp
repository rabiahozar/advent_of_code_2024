#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>

bool isUpdateValid(std::map<int, std::vector<int>>& rules, const std::vector<int>& update)
{
    for (size_t i = 0; i < update.size(); i++) {
        for (size_t j = 0; j < update.size(); j++) {
            // std::cout << "updatei " << update[i] << " , updatej " << update[j] << std::endl;
            if (i == j) {
                continue;
            } else if (j > i) {
                std::vector<int> rule = rules[update[j]];
                if (std::find(rule.begin(), rule.end(), update[i]) != rule.end()) {
                    return false;
                }
            } else if (i > j) {
                std::vector<int> rule = rules[update[i]];
                // for (auto ru : rule) {
                //     std::cout << ru << " , ";
                // }
                // std::cout << std::endl;
                if (std::find(rule.begin(), rule.end(), update[j]) != rule.end()) {
                    return false;
                }
            }
        }
    }
    return true;
}

int fixUpdateAndReturnMid(std::map<int, std::vector<int>>& rules, const std::vector<int>& update)
{
    std::vector<int> correctUpdate{*update.begin()};

    for (size_t i =1 ; i < update.size(); i++){
        // go over existing items and put number in place
        auto newElementShouldBePlacedBefore = rules[update[i]];
        size_t firstPossiblePosition = 0;
        for (size_t j =0 ; j < correctUpdate.size(); j++){
            auto newElementShouldBePlacedAfter = rules[correctUpdate[j]];
            if (std::find(newElementShouldBePlacedAfter.begin(), newElementShouldBePlacedAfter.end(), update[i])!= newElementShouldBePlacedAfter.end()){
                firstPossiblePosition = j+1;
            } else if (std::find(newElementShouldBePlacedBefore.begin(), newElementShouldBePlacedBefore.end(), correctUpdate[j])!= correctUpdate.end()){
                firstPossiblePosition= j;
                break;
            } else {
                firstPossiblePosition = j;
            }
        }
        if (firstPossiblePosition >= correctUpdate.size()) {
            correctUpdate.push_back(update[i]);
        } else {
            correctUpdate.insert(correctUpdate.begin()+firstPossiblePosition, update[i]);
        }
    }

    return correctUpdate.empty() ? 0 : *(correctUpdate.begin() + correctUpdate.size() / 2);
}

int main(int argc, char* argv[])
{
     bool mini_input = false;
    
    if (argc >= 2) {
        if (std::string(argv[1]) == "mini") {
            mini_input = true;
        }
    }

    std::string fileName = mini_input ? "../mini_input" : "../input";
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return 1;
    }

    std::map<int, std::vector<int>> rules;
    std::string line;
    std::regex re(R"(\d+|\d+)");
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }
        std::regex_iterator<std::string::iterator> rit(line.begin(), line.end(), re);
        std::regex_iterator<std::string::iterator> rend;
        if (rit != rend) {
            std::string int1 = rit->str();
            ++rit;
            std::string int2 = rit->str();
            if (rules.count(std::stoi(int1)) == 0) {
                rules[std::stoi(int1)] = std::vector<int>();
            }
            rules[std::stoi(int1)].push_back(std::stoi(int2));
        }
    }

    std::vector<std::vector<int>> updates;
    std::string delimiter = ",";
    while (std::getline(file, line)) {
        size_t pos = 0;
        std::string token;
        std::vector<int> up;
        while ((pos = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, pos);
            up.push_back(std::stoi(token));
            line.erase(0, pos + delimiter.length());
        }
        up.push_back(std::stoi(line));
        updates.push_back(up);
    }

    size_t i = 1;
    long long middleSum = 0;
    long long correctedMiddleSum = 0 ;
    for (const auto& update : updates) {
        if (isUpdateValid(rules, update)) {
            // std::cout << "#" << i << " is valid" << std::endl;
            middleSum += *(update.begin() + update.size() / 2);
        } else {
            // std::cout << "#" << i << " is not valid" << std::endl;
            correctedMiddleSum += fixUpdateAndReturnMid(rules, update);
        }
        i++;
    }

    std::cout << "Result middle sum: " << middleSum << std::endl;
    std::cout << "Result corrected middle sum: " << correctedMiddleSum << std::endl;

    return 0;
}