#pragma once

#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>
#include <map>


// Function that returns the logarithmic return for a given time period
std::vector<long double> logarithmic_return_function(std::vector<long double>& price) {
    std::vector<long double> r_t_list;
    
    // Using a regular iterator to loop through all the initial prices
    for (std::vector<long double>::iterator it = price.begin(); it != price.end()-1; ++it) {
        long double r_t = log(*(it + 1) / *it);
        r_t_list.push_back(r_t);
    
    }
    return r_t_list; // Assuming you want to return the average of r_t_list
};


// Function that returns the average of a vector of doubles
// Return datatype: double 
long double average(std::vector<long double>& return_list) {
    long double average = std::accumulate(return_list.begin(), return_list.end(), 0.0) / return_list.size();
    return average;
};


// Function that returns the average return for a given time period
long double average_return(std::vector<long double> r_average_list) {
    long double r_bar = average(r_average_list);
    return r_bar;
};


// Function that returns a vector of the difference between a vector of 
// log_return for that time period and the average return for the same time period
long double iter_variance(const std::vector<long double>& log_return_for_time_period, long double average_return_for_time_period) {
        long double variance_result = 0.0;
        int count = 0;
        for (std::vector<long double>::const_iterator it = log_return_for_time_period.begin(); it != log_return_for_time_period.end(); ++it) {
            variance_result = std::pow((*it - average_return_for_time_period), 2.0) + variance_result;
            count++;
        };

        return variance_result;

};


// Function that returns the volatility for a given time period: Hourly in this case
long double volatility(std::vector<long double>& log_return, long double average) {
    long double variance = iter_variance(log_return, average)/(log_return.size() - 1);
    long double volatility = std::sqrt(variance);
    return volatility;
};

// EWMA formula to update the volatility for a given time period 
long double update_volatility(long double old_volatility, long double new_price, long double old_price, long double lambda) {
    // Calculate the log return
    long double r_t = log(new_price / old_price);
    
    // Update variance using EWMA formula
    long double old_variance = std::pow(old_volatility, 2.0);
    long double new_variance = (1.0 - lambda) * std::pow(r_t, 2.0) + lambda * old_variance;
    
    // Return the updated volatility (square root of variance)
    return std::sqrt(new_variance);
};

// Function that returns the volatility for a given time period: Hourly for 6 consecutive hours
long double volatility_algorithm(std::vector<long double>& stock_prices) {
        std::vector<long double> log_returns = logarithmic_return_function(stock_prices);
        long double avg_return = average_return(log_returns);
        long double vol = volatility(log_returns, avg_return);
        return vol;
};

