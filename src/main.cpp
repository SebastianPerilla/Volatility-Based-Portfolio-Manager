#include "volatility_formula.h"
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <numeric>


// Function that returns a map with they key as the ticker(string) and the value as the volatility(double) for the given time period: Hourly in this case
std::map<std::string, double> ticker_to_vol_hourly(std::map<std::string, std::vector<double>> input_map) {
    
    std::map<std::string, double> ticker_vol_map;

    for (const auto& pair : input_map) {
        const std::string& ticker = pair.first;
        const std::vector<double>& prices = pair.second;

        if (prices.empty()) {
            std::cout << " No data for " << ticker << std::endl;
            continue;
        }

        // Calculation of Volatility
        std::vector<double> log_returns = logarithmic_return_function(const_cast<std::vector<double>&>(prices));
        double avg_return = average_return(log_returns);
        double vol = volatility(log_returns, avg_return);

        ticker_vol_map[ticker] = vol * std::sqrt(6);

        // std::cout << " Ticker: " << ticker << std::endl;
        // std::cout << " Average return: " << avg_return << std::endl;

        // // Prints the log returns
        // // for (auto& i : log_returns) {
        // //     std::cout << " "<< i << "\n";
        // // }

        // std::cout << std::endl;
        // std::cout << " Variances: " << iter_variance(log_returns, avg_return) << std::endl;
        // std::cout << " Volatility: " << vol << std::endl;
        // std::cout << " Daily Volatility Estimate: " << vol * std::sqrt(24) << std::endl;
        // std::cout << std::endl;
    }



    std::cout << "Daily Ticker Volatility Map: " << std::endl;
    for (const auto& pair : ticker_vol_map) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    };

    return ticker_vol_map;
};


//Function with the main code testing each portion of the volatility calculation
int main() {

    std::map<std::string, std::vector<double>> standard_volatility = {
        {"NVDA", {100, 200, 300, 400, 500, 600}},
        {"MSFT", {1, 2, 3, 4, 5, 6, 7, 200}},
        {"TSLA", {1, 1, 1, 1, 1, 1, 1, 100}},
        {"AAPL", {}},
        {"GOOG", {}},
        {"META", {}},
        {"AVGO", {}},
    };


    // // Two pointers
    // for (const auto& pair : price_map) {
    //     const std::string& ticker = pair.first;
    //     const std::vector<double>& prices = pair.second;

    //     std::cout << "Ticker: " << ticker << std::endl;

    //     if (prices.size() < 2) {
    //         std::cout << "Not enough data for " << ticker << std::endl;
    //         continue;
    //     }

    //     for (size_t i = 0; i < prices.size() - 1; ++i) {
    //         double first_value = prices[i];
    //         double second_value = prices[i + 1];
    //         double sum = first_value + second_value;

    //         std::cout << "Sum of " << first_value << " and " << second_value << " is " << sum << std::endl;
    //     }

    //     std::cout << std::endl;
    // }

    std::map<std::string, double> output = ticker_to_vol_hourly(standard_volatility); 

    std::cout << "\nFinal Ticker Volatility Map: " << std::endl;

    for (const auto& pair : output) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    };

    return 0;
}
