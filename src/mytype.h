// include/mytype.h
#pragma once
#include <iostream>
#include <fstream>
namespace homework {

class MyType {
    int i;
    double d;

public:
    MyType(int i, double d) : i(i), d(d) {}

    auto operator==(const MyType& other) const {
        return i == other.i && d == other.d;
    }

    // Friend function for output (write to file)
    friend std::ostream& operator<<(std::ostream& os, const MyType& mt) {
        os << mt.i << " " << mt.d;
        return os;
    }

    // Friend function for input (read from file)
    friend std::istream& operator>>(std::istream& is, MyType& mt) {
        is >> mt.i >> mt.d;
        return is;
    }
};

void writeToFile(const MyType& mt, const std::string& filename) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << mt;
        outFile.close();
    } else {
        throw std::runtime_error("Could not open file for writing");
    }
}

MyType readFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    MyType mt(0, 0);  // Default initialization
    if (inFile.is_open()) {
        inFile >> mt;
        inFile.close();
    } else {
        throw std::runtime_error("Could not open file for reading");
    }
    return mt;
}

}