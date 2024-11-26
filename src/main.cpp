#include "volatility_formula.h"
#include "volatility_parse.h"
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <numeric>


//Function with the main code testing each portion of the volatility calculation
int main() {

    std::map<std::string, std::vector<long double>> standard_volatility = {
        {"NVDA", {100, 200, 300, 400, 500, 600, 220, 500, 200, 400, 700, 100, 900}},
        {"MSFT", {100, 200, 300, 400, 500, 600, 220, 500, 200, 400, 700, 100, 900}},
        {"TSLA", {47.66699981689453, 48.11602020263672,48.10852813720703, 48.185997009277344, 48.1879997253418, 47.88399887084961, 48.185001373291016, 47.49448776245117, 47.751502990722656, 47.773990631103516, 47.62432861328125, 47.69300079345703, 47.652000427246094}},
        {"AAPL", {}},
        {"GOOG", {}},
        {"META", {}},
        {"AVGO", {}},
    };


    std::map<std::string, long double> output = ticker_to_vol_hourly(standard_volatility);
    std::map<std::string, std::vector<long double>> true_vol = true_volatility(standard_volatility, output);

    // for (const auto& pair : output) {
    //     const std::string& ticker = pair.first;
    //     const long double& vol = pair.second;

    //     std::cout << "\n Ticker: " << ticker << std::endl;
    //     std::cout << " Volatility: " << vol << std::endl;
    // }

    for (const auto& pair : true_vol) {
        const std::string& ticker = pair.first;
        const std::vector<long double>& prices = pair.second;

        std::cout << "\n Ticker: " << ticker << std::endl;
        std::cout << " Hourly Volatilities: " << std::endl;
        for (auto& i : prices) {
            std::cout << " " << i << "\n";
        }
    }

    return 0;
}
