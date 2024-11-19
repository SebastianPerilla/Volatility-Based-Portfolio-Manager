#include<cmath>
#include<iostream>
#include <vector>
#include <numeric>


double average(std::vector<double>& return_list) {
    double average = std::accumulate(return_list.begin(), return_list.end(), 0.0) / return_list.size();
    return average;
};


void checker(){
    std::vector<int> values = {100, 210, 320, 430, 540};
    std::vector<std::string> week = {"monday", "tuesday", "wednesday", "thursday", "friday"}; 

    // Using a regular iterator
    for (std::vector<int>::iterator it = values.begin(); it != values.end()-1; ++it) {
        
        
        std::cout << week[(it +1) - values.begin()] << ": " << *(it + 1) - *it << " ";
    }
    std::cout << std::endl;

    // Using a range-based for loop (C++11 and later)
    for (int value : values) {
        std::cout << values[value] << " ";
    }
    std::cout << std::endl;
};



// Function that returns a vector of the difference between a vector of 
// log_return for that time period and the average return for the same time period
double iter_variance(const std::vector<double>& log_return_for_time_period, double average_return_for_time_period) {
        // std::vector<double> variance_result;
        double variance_result = 0.0;
        int count = 0;
        for (std::vector<double>::const_iterator it = log_return_for_time_period.begin(); it != log_return_for_time_period.end(); ++it) {
            std::cout<< *it - average_return_for_time_period<< std::endl;
            variance_result = std::pow((*it - average_return_for_time_period), 2.0) + variance_result;
            // std::cout << "Value " << count << " : "<< variance_result << std::endl;

            count++;
        };
        return variance_result;

};




// for (std::vector<double>::iterator it = price.begin(); it != price.end()-1; ++it) {
//         double r_t = log(*(it + 1) / *it);
//         std::cout << week[(it + 1) - price.begin()] << ": " << *(it + 1) - *it << " ";
//         r_t_list.push_back(r_t);
    
//     }
//     std::cout << std::endl;
    


int main() {
    // checker();
    double result = iter_variance({4.0, 6.0, 8.0}, 2.0);
    std::cout << result << std::endl;
    return 0;
}