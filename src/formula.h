#pragma once
#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>



// Function that returns the average of a vector of doubles
// Return datatype: double 
double average(std::vector<double>& return_list) {
    double average = std::accumulate(return_list.begin(), return_list.end(), 0.0) / return_list.size();
    return average;
};


// Function that returns a vector of the difference between a vector of 
// log_return for that time period and the average return for the same time period
double iter_variance(const std::vector<double>& log_return_for_time_period, double average_return_for_time_period) {
        double variance_result = 0.0;
        int count = 0;
        for (std::vector<double>::const_iterator it = log_return_for_time_period.begin(); it != log_return_for_time_period.end(); ++it) {
            // std::cout<< *it - average_return_for_time_period<< std::endl;
            variance_result = std::pow((*it - average_return_for_time_period), 2.0) + variance_result;

            count++;
        };
        return variance_result;

};


// Function that returns the logarithmic return for a given time period
std::vector<double> logarithmic_return_function(std::vector<double>& price) {
    std::vector<double> r_t_list;
    
    // Using a regular iterator
    for (std::vector<double>::iterator it = price.begin(); it != price.end()-1; ++it) {
        double r_t = log(*(it + 1) / *it);
        // std::cout << *(it + 1) - *it << " ";
        r_t_list.push_back(r_t);
    
    }
    std::cout << std::endl;
    return r_t_list; // Assuming you want to return the average of r_t_list
};


// Function that returns the average return for a given time period
double average_return(std::vector<double> r_average_list) {
    double r_bar = average(r_average_list);
    return r_bar;
};


// Function that returns the volatility for a given time period: Hourly in this case
double volatility(std::vector<double>& log_return, double average) {
    double variance = iter_variance(log_return, average)/log_return.size();
    double volatility = std::sqrt(variance);
    return volatility;
};
