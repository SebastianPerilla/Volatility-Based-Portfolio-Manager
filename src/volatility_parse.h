#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cmath>

// Function declarations
std::vector<double> logarithmic_return_function(const std::vector<double>& prices);
double average_return(const std::vector<double>& log_returns);
double volatility(const std::vector<double>& log_returns, double avg_return);
double update_volatility(double current_volatility, double new_price, double old_price, double lambda);
double volatility_algorithm(std::vector<double>& stock_prices);


struct Print_values {
    void print_map(std::map<std::string, double> value) const {
        for (const auto& pair : value) {
            std::cout << pair.first << pair.second << std::endl;
        }
    }

    void print_vector(std::vector<double> value) const {
        for (const auto& i : value) {
            std::cout << i << std::endl;
        }
    }

};


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
        double vol_algo = volatility_algorithm(first_six_prices);

        // Adding the value to the initial map for the past volatility calculations
        ticker_vol_map[ticker] = vol_algo;


        // Print Statements incase you want to checkout the values for yourself
        
        

        // std::cout << "\n Ticker: " << ticker << std::endl;
        Print_values print;

        print.print_map(ticker_vol_map);
        
        // std::cout << " Average return: " << avg_return << std::endl;

        // std::cout << "\n Log Returns: " << std::endl;
        // std::cout << " Size: " << log_returns.size() << std::endl;

        // // Prints the log returns
        // for (auto& i : log_returns) {
        //     std::cout << " " << (std::pow(i  - avg_return, 2.0)) << "\n";

        // }

        // std::cout << std::endl;
        // std::cout << " Variances: " << iter_variance(log_returns, avg_return)  << std::endl;
        // std::cout << " Volatility: " << vol_algo << std::endl;
        
    
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
            standard_ticker_vol_map[ticker] = current_volatility;

        } else {
            std::cout << ticker << ": Not enough data" << std::endl;
        }
    }

    return true_volatility_output;
};




