#include "volatility_formula.h"
#include "volatility_parse.h"
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <numeric>


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

    return 0;
}
