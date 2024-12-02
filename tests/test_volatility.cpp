
#include "gtest/gtest.h"
#include <algorithm>
#include <list>
#include <string>
#include <vector>
#include "volatility_formula.h"

namespace VolatilityFunctions {
    
    TEST(Price_Average_Test, Average) {
        std::vector<double> test_vector = {1.0, 2.0, 3.0, 4.0, 5.0};
        EXPECT_EQ(average(test_vector), 3.0);
    }

    TEST(Price_Average_Return_Test, Average_Return) {
        std::vector<double> test_vector = {1.0, 2.0, 3.0, 4.0, 5.0};
        EXPECT_EQ(average_return(test_vector), 3.0);
    }
}