
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

    // Had to use Copilot for this one here

    TEST(Price_Log_Return_Test, Log_Return) {
        std::vector<double> test_vector = {1.0, 2.0, 3.0, 4.0, 5.0};
        std::vector<double> result = logarithmic_return_function(test_vector);

        // Expected values with tolerance (for floating-point comparison)
        std::vector<double> expected_vector = {0.693147, 0.405465, 0.287682, 0.223144};

        for (size_t i = 0; i < result.size(); ++i) {
            EXPECT_NEAR(result[i], expected_vector[i], 1e-5) << "Error at index " << i;
        }
    }

    TEST(VolatilityFunctionsTest, Volatility) {
        std::vector<double> test_vector = {0.693147, 0.405465, 0.287682, 0.223144};
        double average = 0.402869; // Pre-calculated mean
        double variance = iter_variance(test_vector, average) / (test_vector.size() - 1);
        double expected_volatility = std::sqrt(variance); // Manually calculated expected volatility

        double volatility_value = volatility(test_vector, average);

        EXPECT_NEAR(volatility_value, expected_volatility, 0.001);
    }

    TEST(VolatilityFunctionsTest, UpdateVolatility) {
        double old_volatility = 0.05;
        double new_price = 105.0;
        double old_price = 100.0;
        double lambda = 0.94;

        // Expected updated volatility, calculated manually
        double r_t = log(new_price / old_price);
        double old_variance = std::pow(old_volatility, 2.0);
        double new_variance = (1.0 - lambda) * std::pow(r_t, 2.0) + lambda * old_variance;
        double expected_updated_volatility = std::sqrt(new_variance);

        double updated_volatility = update_volatility(old_volatility, new_price, old_price, lambda);

        EXPECT_NEAR(updated_volatility, expected_updated_volatility, 0.001);
    }

}