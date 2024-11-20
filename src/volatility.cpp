#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <numeric>




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




//Function with the main code testing each portion of the volatility calculation
int main() {

    std::map<std::string, std::vector<double>> price_map = {
        {"NVDA", {100, 150, 200, 300, 500}},
        {"MSFT", {226.00010681152344, 226.4499969482422, 225.77000427246094, 226.39999389648438,
                  226.77000427246094, 226.17999267578125, 226.42999267578125, 225.64500427246094, 
                  224.85989379882812, 224.32000732421875, 224.52999877929688, 227.97500610351562, 
                  225.9499969482422, 225.11500549316406, 225.96499633789062, 226.45269775390625, 
                  227.05999755859375, 226.7736053466797, 224.9499969482422, 225.07000732421875, 
                  225.8105010986328, 226.1199951171875, 225.8699951171875, 226.4300994873047, 
                  227.25999450683594, 227.97000122070312, 228.6300048828125, 227.94000244140625, 
                  228.49000549316406, 228.4832000732422, 228.14999389648438, 228.13999938964844}},
        {"TSLA", {249.61000061035156, 250.0, 248.35000610351562, 248.35169982910156, 
                  248.13720703125, 246.77999877929688, 248.39999389648438, 237.93389892578125,
                  239.04010009765625, 240.45860290527344, 240.1658935546875, 241.2530059814453, 
                  239.25010681152344, 238.4199981689453, 240.47500610351562, 241.7449951171875, 
                  241.1999969482422, 241.1699981689453, 239.0500946044922, 239.860107421875, 
                  237.9600067138672, 239.1967010498047, 238.44500732421875, 236.61000061035156, 
                  237.81419372558594, 238.2899932861328, 236.9600067138672, 237.43099975585938, 
                  236.9499969482422, 238.27499389648438, 239.20950317382812, 238.92999267578125, 
                  239.79989624023438, 240.73800659179688, 240.3800048828125, 232.3249969482422, 
                  234.29519653320312, 234.9199981689453, 234.15460205078125, 233.8988037109375, 
                  234.81410217285156, 234.87969970703125, 233.02999877929688, 232.5930938720703, 
                  232.6999969482422, 233.47860717773438, 234.5399932861328, 233.72999572753906, 
                  233.88999938964844, 227.95370483398438, 226.02000427246094, 225.73809814453125, 
                  226.64500427246094, 226.9398956298828}}, 
        {"AAPL", {}}, 
        {"GOOG", {}}, 
        {"META", {}}, 
        {"AVGO", {}},
    };

    for (const auto& pair : price_map) {
        const std::string& ticker = pair.first;
        const std::vector<double>& prices = pair.second;

        if (prices.empty()) {
            std::cout << " No data for " << ticker << std::endl;
            continue;
        }

        std::vector<double> log_returns = logarithmic_return_function(const_cast<std::vector<double>&>(prices));
        double avg_return = average_return(log_returns);
        double vol = volatility(log_returns, avg_return);

        std::cout << " Ticker: " << ticker << std::endl;
        std::cout << " Average return: " << avg_return << std::endl;

        // Prints the log returns
        // for (auto& i : log_returns) {
        //     std::cout << " "<< i << "\n";
        // }

        std::cout << std::endl;
        std::cout << " Variances: " << iter_variance(log_returns, avg_return) << std::endl;
        std::cout << " Volatility: " << vol << std::endl;
        std::cout << " Daily Volatility Estimate: " << vol * std::sqrt(24) << std::endl;
        
        
        std::cout << std::endl;
    }

    return 0;
}
