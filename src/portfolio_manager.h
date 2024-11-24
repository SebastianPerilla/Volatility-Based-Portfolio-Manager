#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility> // For std::pair
struct PortfolioManagerResult {
    std::vector<std::map<std::string, double>> allocations;       // Allocated funds for each stock at each hour
    std::vector<std::map<std::string, double>> portfolio_values;  // Portfolio values at each hour
};

PortfolioManagerResult portfolio_manager(
    const std::vector<std::vector<std::string>>& buying_stocks,
    const std::vector<double>& reallocation_funds,
    std::map<std::string, double>& my_portfolio,
    const std::string& strategy,
    const std::map<std::string, std::vector<double>>& stocks,
    const std::map<std::string, std::vector<double>>& ticker_to_percentage_changes) {
    
    PortfolioManagerResult result;

    if (buying_stocks.empty() || reallocation_funds.empty()) {
        return result; // Empty result if no stocks to buy or funds
    }

    size_t hours = buying_stocks.size();

    for (size_t hour = 0; hour < hours; ++hour) {
        // **Update portfolio for market changes at the start of each hour**
        for (auto& [stock, value] : my_portfolio) {
            // Check if percentage changes exist for this stock
            if (ticker_to_percentage_changes.count(stock)) {
                const auto& percentage_changes = ticker_to_percentage_changes.at(stock);

                // Check if there’s a percentage change for the current hour
                if (hour < percentage_changes.size()) {
                    double percentage_change = percentage_changes[hour];
                    value *= (1.0 + (percentage_change / 100.0)); // Apply percentage change
                }
            }
        }

        // Allocation for the current hour
        std::map<std::string, double> hour_allocation;

        // Skip this hour if no buying stocks or reallocation funds
        if (buying_stocks[hour].empty() || reallocation_funds[hour] <= 0) {
            // Store current portfolio values
            result.portfolio_values.push_back(my_portfolio);
            // Even if no allocation happened, store an empty allocation
            result.allocations.push_back(hour_allocation);
            continue;
        }

        // Determine weights for allocation based on strategy and average volatility
        std::map<std::string, double> allocation_weights;
        double total_weight = 0.0;

        for (const auto& stock : buying_stocks[hour]) {
            const auto& volatility_values = stocks.at(stock);
            double avg_volatility = 0.0;

            // Calculate average volatility for the stock
            for (double vol : volatility_values) {
                avg_volatility += vol;
            }
            avg_volatility /= volatility_values.size();

            double weight = 0.0;

            if (strategy == "optimistic") {
                weight = 1.0 / (avg_volatility + 0.001); // Inverse relation to volatility
            } else if (strategy == "neutral") {
                weight = 1.0;
            } else if (strategy == "conservative") {
                weight = 1.0 / (avg_volatility + 0.0005); // Stronger inverse relation
            }

            allocation_weights[stock] = weight;
            total_weight += weight;
        }

        // Allocate funds proportionally based on weights
        for (const auto& stock : buying_stocks[hour]) {
            double weight = allocation_weights[stock];
            double allocation = (weight / total_weight) * reallocation_funds[hour];

            // Update the portfolio with the allocated funds
            my_portfolio[stock] += allocation;

            // Store the allocation result
            hour_allocation[stock] = allocation;
        }

        // Add the allocation for this hour to the result
        result.allocations.push_back(hour_allocation);

        // Store the current state of my_portfolio
        result.portfolio_values.push_back(my_portfolio);
    }

    return result;
}



