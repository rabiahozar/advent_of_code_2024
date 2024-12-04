#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>
#include <set>

int main(int argc, char* argv[]) 
{
    bool mini_input = false;
    
    if (argc >= 2) {
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
    std::vector<std::string> puzzle;
    while (std::getline(infile, line)) {
        puzzle.push_back(line);
    }

    size_t wordCount = 0;
    // look for every line for regular

    std::regex re("XMAS");
    std::regex re2("SAMX");

    std::string xmas("XMAS");
    std::string samx("SAMX");

    for (size_t i = 0; i < puzzle.size(); i++) {
        std::string subject = puzzle[i];
        std::smatch match;
        while (std::regex_search(subject, match, re)) {
            wordCount++;
            subject = match.suffix().str();
        }
    }
    // look for every line SAMX

    for (size_t i = 0; i < puzzle.size(); i++) {
        std::string subject = puzzle[i];
        std::smatch match;
        while (std::regex_search(subject, match, re2)) {
            wordCount++;
            subject = match.suffix().str();
        }
    }

    auto searchVertical = [&](const std::string& word) {
        if (puzzle.size() >= 4) {
            for (size_t j = 0; j < puzzle[0].size(); j++) {
                for (size_t i = 0; i <= puzzle.size() - 4; i++) {
                    size_t local = i;
                    bool match = true;
                    for (auto x : word) {
                        if (x != puzzle[local][j]) {
                            match = false;
                            break;
                        }
                        local++;
                    }
                    if (match) {
                        wordCount++;
                    }
                }
            }
        }
    };

    // look for vertical XMAS
    searchVertical(xmas);
    // look for vertical SAMX
    searchVertical(samx);

    auto searchDiagonal = [&](int dx, int dy, std::string word) {
        for (size_t i = 0; i <= puzzle.size() - 4; i++) {
            for (size_t j = (dy > 0 ? 0 : 3); j <= puzzle[i].size() - (dy > 0 ? 4 : 1); j++) {
                bool match = true;
                size_t localx = i;
                size_t localy = j;
                for (auto ch : word) {
                    if (ch != puzzle[localx][localy]) {
                        match = false;
                        break;
                    }
                    localx += dx;
                    localy += dy;
                }
                if (match) {
                    wordCount++;
                }
            }
        }
    };

    // look for diagonal i+1 j+1
    searchDiagonal(1, 1, xmas);
    // look for diagonal i+1 j-1
    searchDiagonal(1, -1, xmas);
    // look for diagonal i+1 j+1 reversed
    searchDiagonal(1, 1, samx);
    // look for diagonal i+1 j-1 reversed
    searchDiagonal(1, -1, samx);
    std::cout << "Result xmas: " << wordCount << std::endl;

    std::set<std::pair<int, int>> coordinates;

    auto searchDiagonal2 = [&](int dx, int dy, const std::string& word) {
        for (size_t i = 0; i <= puzzle.size() - 3; i++) {
            for (size_t j = (dy > 0 ? 0 : 2); j <= puzzle[i].size() - (dy > 0 ? 3 : 1); j++) {
                bool match = true;
                size_t localx = i;
                size_t localy = j;
                for (char ch : word) {
                    if (ch != puzzle[localx][localy]) {
                        match = false;
                        break;
                    }
                    localx += dx;
                    localy += dy;
                }
                if (match) {
                    coordinates.emplace(i, j);
                    coordinates.emplace(localx - dx, localy - dy);
                }
            }
        }
    };

    std::string lookingMAS = "MAS";
    std::string lookingSAM = "SAM";

    searchDiagonal2(1, 1, lookingMAS);
    searchDiagonal2(1, -1, lookingMAS);
    searchDiagonal2(1, 1, lookingSAM);
    searchDiagonal2(1, -1, lookingSAM);

    size_t X_MASCount = 0;
    for (const auto& coordinate : coordinates) {
        int x = coordinate.first;
        int y = coordinate.second;

        if (x + 2 >= puzzle.size() || y + 2 >= puzzle[0].size() || y - 2 < 0) {
            continue; // Skip out-of-bound checks
        }

        // Define the edges of the "X-MAS"
        std::pair<int, int> topRightEdge{x, y + 2};
        std::pair<int, int> bottomLeftEdge{x + 2, y};
        std::pair<int, int> bottomRightEdge{x + 2, y + 2};
        std::pair<int, int> bottomLeftAltEdge{x + 2, y - 2};

        // Check both possible diagonal setups
        bool isXMAS1 = coordinates.count(topRightEdge) && coordinates.count(bottomLeftEdge) && coordinates.count(bottomRightEdge);
        bool isXMAS2 = coordinates.count(topRightEdge) && coordinates.count(bottomLeftAltEdge) && coordinates.count(bottomRightEdge);

        if (isXMAS1 || isXMAS2) {
            X_MASCount++;
        }
    }

    // finds too high see part2.cpp
    // try to previous logic did not fit
    std::cout << "Result X-MAS: " << X_MASCount << std::endl;

    return 0;
}