#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>

bool isSafe(const std::vector<int>& report)
{
    const int maxIncrease = 3;
    const int minIncrease = 1; 
    bool isIncreasing = true;
    bool isDecreasing = true;

    for (size_t i = 1; i < report.size(); ++i) {

        int diff = report[i] - report[i-1];

        if (std::abs(diff) > maxIncrease || std::abs(diff) < minIncrease){
            return false;
        }

        if (diff > 0) {
            isDecreasing = false;
        } else {
            isIncreasing = false;
        }

        if (not isIncreasing and not isDecreasing)  return false;
    }
    return true;
}

bool canReportBeFixed(const std::vector<int>& report)
{
    for (size_t i = 0; i < report.size(); ++i) {
        std::vector<int> copyReport;

        for (size_t j = 0; j < report.size(); ++j) {
            if (j != i) {
                copyReport.push_back(report[j]);
            }
        }

        if (isSafe(copyReport)) {
            return true;
        }
    }
    return false;
}
void countSafeReports(std::vector<std::vector<int>>& reports)
{
    long long int safeReportCount = 0;

    for (const auto& report: reports) {
        if(isSafe(report)) {
            safeReportCount++;
        }
    }

    std::cout << "Safe report count: " << safeReportCount << std::endl;
}

void countSafeReportsWithProblemDampener(std::vector<std::vector<int>>& reports)
{
    int safeReportCount = 0;

    for (const auto& report: reports) {
        if(isSafe(report)) {
            safeReportCount++;
        } else if (canReportBeFixed(report)) {
            safeReportCount++;
        }
    }

    std::cout << "Safe report count with fix: " << safeReportCount << std::endl;
}


int main()
{
    std::ifstream infile("../input");
    if (!infile) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return 1;
    }
    
    std::vector<std::vector<int>> reports;

    std::string line;

    while (std::getline(infile, line)) {
        reports.push_back({});
        std::istringstream iss(line);
        int num1;
        while (iss >> num1) {
            reports.back().push_back(num1);
        }
    }
    
    countSafeReports(reports);
    countSafeReportsWithProblemDampener(reports);
    return 0;
}