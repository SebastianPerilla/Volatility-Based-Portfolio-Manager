#include "volatility_formula.h"
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <numeric>


/*
Function that returns a map with they key as the ticker(string) and the value as the 
volatility(double) for the given time period: Hourly in this case
*/
std::map<std::string, double> ticker_to_vol_hourly(std::map<std::string, std::vector<double>> input_map) {
    
    std::map<std::string, double> ticker_vol_map;

    for (const auto& pair : input_map) {
        const std::string& ticker = pair.first;
        const std::vector<double>& prices = pair.second;

        if (prices.size() < 6) {
            std::cout << " Not enough data for " << ticker << std::endl;
            continue;
        }

        // Use only the first 6 values
        std::vector<double> first_six_prices(prices.begin(), prices.begin() + 6);

        // Calculation of Volatility
        std::vector<double> log_returns = logarithmic_return_function(first_six_prices);
        double avg_return = average_return(log_returns);
        double vol = volatility(log_returns, avg_return);

        // Adding the value to the initial map for the past volatility calculations
        ticker_vol_map[ticker] = vol;


        // Print Statements incase you want to checkout the values for yourself
        

        std::cout << "\n Ticker: " << ticker << std::endl;
        // std::cout << " Average return: " << avg_return << std::endl;

        // std::cout << "\n Log Returns: " << std::endl;
        // std::cout << " Size: " << log_returns.size() << std::endl;

        // // Prints the log returns
        // for (auto& i : log_returns) {
        //     std::cout << " " << (std::pow(i  - avg_return, 2.0)) << "\n";

        // }

        // std::cout << std::endl;
        // std::cout << " Variances: " << iter_variance(log_returns, avg_return)  << std::endl;
        std::cout << " Volatility: " << vol << std::endl;
        
    
    }

    return ticker_vol_map;
}


std::map<std::string, double> true_volatility(std::map<std::string, std::vector<double>> input_map, std::map<std::string, double> standard_ticker_vol_map){
    
    std::cout << "\n-----------------------------------\n";

    std::map<std::string, double> true_volatility_output;
            
    for (const auto& pair : standard_ticker_vol_map) {
        const std::string& ticker = pair.first;
        const std::vector<double>& prices = input_map[ticker];

        if (prices.size() > 6) {
            double current_volatility = pair.second;
            double lambda = 0.94;

            for (size_t i = 5; i < prices.size()-1; ++i) {
                double old_price = prices[i];
                double new_price = prices[i + 1];
                current_volatility = update_volatility(current_volatility, new_price, old_price, lambda);
            //     std::cout << "\nTicker: " << ticker << std::endl;
            // std::cout << "Current Volatility (Updated): " << current_volatility << std::endl;
            }

            std::cout << "\nTicker: " << ticker << std::endl;
            std::cout << "Current Volatility (Updated): " << current_volatility << std::endl;
            true_volatility_output[ticker] = current_volatility;

        } else {
            std::cout << ticker << ": Not enough data" << std::endl;
        }
    }

    return true_volatility_output;
};



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
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}
