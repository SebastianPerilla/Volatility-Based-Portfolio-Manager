#include "volatility_formula.h"
#include "volatility_parse.h"
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <numeric>


// // Dummy implementation of true_volatility for demonstration purposes
// std::map<std::string, double> true_volatility(const std::map<std::string, std::vector<double>>& standard_volatility, const std::map<std::string, double>& output) {
//     std::map<std::string, double> true_vol_map;

//     for (const auto& pair : standard_volatility) {
//         const std::string& ticker = pair.first;
//         const std::vector<double>& prices = pair.second;

//         if (prices.empty()) {
//             true_vol_map[ticker] = 0.0;
//             continue;
//         }

//         // Example calculation (replace with actual logic)
//         double vol = output.at(ticker) * 1.1; // Just an example multiplier
//         true_vol_map[ticker] = vol;

//         // Debug print statements
//         // std::cout << "Ticker: " << ticker << ", Volatility: " << vol << std::endl;
//     }

//     return true_vol_map;
// }


//Function with the main code testing each portion of the volatility calculation
int main() {

    std::map<std::string, std::vector<double>> standard_volatility = {
        {"NVDA", {100, 200, 300, 400, 500, 600, 220, 500, 200, 400, 700, 100, 900}},
        {"MSFT", {100, 200, 300, 400, 500, 600, 220, 500, 200, 400, 700, 100, 900}},
        {"TSLA", {100, 200, 300, 400, 500, 600, 220, 500, 200, 400, 700, 100, 900}},
        {"AAPL", {}},
        {"GOOG", {}},
        {"META", {}},
        {"AVGO", {}},
    };


    std::map<std::string, double> output = ticker_to_vol_hourly(standard_volatility);
    std::map<std::string, double> true_vol = true_volatility(standard_volatility, output);
    
    for (const auto& pair : true_vol) {
        std::cout << "Ticker: " << pair.first << ", True Volatility: " << pair.second << std::endl;
    }

    return 0;
}
