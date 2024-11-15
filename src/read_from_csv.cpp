#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


int main() {
    std::ifstream file("data.csv");  // Open the CSV file
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {  // Read the file line by line
        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> row;

        // Split the line by commas and store each field in a vector
        while (std::getline(ss, field, ',')) {
            row.push_back(field);
        }

        // Process the row (printing it in this case)
        for (const auto& f : row) {
            std::cout << f << " ";
        }
        std::cout << std::endl;
    }

    file.close();  // Close the file
    return 0;
}
