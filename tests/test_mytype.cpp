//tests/test_mytype.cpp
#include "gtest/gtest.h"
#include "mytype.h"
#include <fstream>


using namespace homework;

TEST(MyType, WriteRead){
    MyType mt(42,3.14);
    writeToFile(mt,"mytype.txt");
    auto mt2 = readFromFile("mytype.txt");
    ASSERT_EQ(mt,mt2);
}