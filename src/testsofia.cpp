#include <iostream>
#include <map>
#include <vector>
#include <string>

// Function to process a specific hourly map
void process_hour_map(const std::map<std::string, long double>& hourly_map, size_t hour) {
    std::cout << "Processing hour: " << hour << "\n";

    // Display the hourly map
    for (const auto& ticker_pair : hourly_map) {
        std::cout << "  Ticker: " << ticker_pair.first << ", Price: " << ticker_pair.second << "\n";
    }
    std::cout << "-----------------------------------\n";
}

int main() {
    // Example data: ticker_to_prices
    std::map<std::string, std::vector<long double>> ticker_to_prices = {
        {"AAPL", {150.0L, 151.2L, 152.4L, 153.0L, 154.5L, 155.2L}},
        {"GOOGL", {2800.5L, 2810.3L, 2820.8L, 2830.0L, 2840.5L, 2850.2L}}
    };

    // Determine the maximum number of hours
    size_t max_hours = 0;
    for (const auto& pair : ticker_to_prices) {
        max_hours = std::max(max_hours, pair.second.size());
    }

    // Loop through each hour and create the hourly map
    for (size_t hour = 0; hour < max_hours; ++hour) {
        // Create a new map for the current hour
        std::map<std::string, long double> hourly_map;

        // Populate the map for this specific hour
        for (const auto& pair : ticker_to_prices) {
            const std::string& ticker = pair.first; // Key: ticker
            const std::vector<long double>& prices = pair.second; // Value: vector of prices

            // Only add the price if it exists for this hour
            if (hour < prices.size()) {
                hourly_map[ticker] = prices[hour];
            }
        }

        // Pass the hourly map to the processing function
        process_hour_map(hourly_map, hour);
    }

    return 0;
}
