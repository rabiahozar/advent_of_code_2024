#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>

void calculateDistance(std::vector<int> list1, std::vector<int> list2) {
    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    long long distance = 0;
    auto it1 = list1.begin(), it2 = list2.begin();

    while (it1 != list1.end() && it2 != list2.end()) {
        distance += std::abs(*it1 - *it2);
        ++it1;
        ++it2;
    }

    // Handle remaining elements in longer list
    while (it1 != list1.end()) distance += *it1++;
    while (it2 != list2.end()) distance += *it2++;

    std::cout << "Total Distance: " << distance << std::endl;
}

void calculateSimilarity(std::vector<int> list1, std::vector<int> list2)
{
    std::set<int> count1;
    std::map<int, int> count2;

    for (auto point: list1)
    {
        count1.emplace(point);
    }

    for (auto point: list2)
    {
        count2[point]++;
    }

    long long int similarity = 0;

    for (auto point : count1) {
        if (count2.find(point) != count2.end()){
            similarity += count2[point] *point;
        }
    }

    std::cout << "Total Similarity: " << similarity << std::endl;
}

int main() {
    std::ifstream infile("../input");
    if (!infile) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return 1;
    }

    std::vector<int> list1, list2;
    std::string line;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        int num1, num2;
        if (iss >> num1 >> num2) {
            list1.push_back(num1);
            list2.push_back(num2);
        } else {
            std::cerr << "Error: Malformed line in input file." << std::endl;
        }
    }

    infile.close();

    calculateDistance(list1, list2);
    calculateSimilarity(list1, list2);

    return 0;
}

