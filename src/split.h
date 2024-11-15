#pragma once
#include <sstream>
#include <string>
#include <vector>

namespace homework {
    std::vector<std::string> split(const std::string& str) {
        std::vector<std::string> result;
        std::stringstream ss(str);
        std::string word;

        while (ss >> word) {
            result.push_back(word);
        }

        return result;
    }
} // namespace homework
