#include<cmath>
#include<iostream>
#include <vector>
#include <numeric>
#include <map>


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
        double variance_result = 0.0;
        int count = 0;
        for (std::vector<double>::const_iterator it = log_return_for_time_period.begin(); it != log_return_for_time_period.end(); ++it) {
            std::cout<< *it - average_return_for_time_period<< std::endl;
            variance_result = std::pow((*it - average_return_for_time_period), 2.0) + variance_result;

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

    std::map<std::string, std::vector<double>> price_map = {
    {"NVDA", {47.66699981689453, 48.11602020263672, 48.10852813720703, 48.185997009277344, 
              48.1879997253418, 47.88399887084961, 48.185001373291016, 47.49448776245117, 
              47.751502990722656, 47.773990631103516, 47.62432861328125, 47.69300079345703, 
              47.652000427246094, 47.56999969482422, 47.95499801635742, 48.305660247802734, 
              48.26900100708008, 48.279998779296875, 48.14400100708008, 48.19586944580078, 
              47.99399948120117, 48.95199966430664, 49.38750076293945, 49.04125213623047, 
              49.15599822998047, 49.209938049316406, 48.868499755859375, 49.09000015258789, 
              51.32600021362305, 51.28200149536133}},
    {"MSFT", {226.00010681152344, 226.4499969482422, 225.77000427246094, 226.39999389648438,
              226.77000427246094, 226.17999267578125, 226.42999267578125, 225.64500427246094, 
              224.85989379882812, 224.32000732421875, 224.52999877929688, 227.97500610351562, 
              225.9499969482422, 225.11500549316406, 225.96499633789062, 226.45269775390625, 
              227.05999755859375, 226.7736053466797, 224.9499969482422, 225.07000732421875, 
              225.8105010986328, 226.1199951171875, 225.8699951171875, 226.4300994873047, 
              227.25999450683594, 227.97000122070312, 228.6300048828125, 227.94000244140625, 
              228.49000549316406, 228.4832000732422, 228.14999389648438, 228.13999938964844}},
    {"TSLA", {}}, 
    {"AAPL", {}}, 
    {"GOOG", {}}, 
    {"META", {}}, 
    {"AVGO", {}},
    };
    
    for (const auto& pair : price_map) {
        
        std::cout << pair.first << ": ";
        for (const auto& price : pair.second) {
            std::cout << price << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    

    // double result = iter_variance({4.0, 6.0, 8.0}, 2.0);
    // std::cout << result << std::endl;
    return 0;
}