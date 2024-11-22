#include "volatility_formula.h"
#include "portfolio_manager.h"
#include "stock_manager.h"
#include "volatility_parse.h"
#include "extractor.h"
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <numeric>
#include <string>
#include <utility> // For std::pair
#include <tuple>
#include <limits>
#include <algorithm>
#include <cctype>


/* FUNCTION TO INITIALISE GAME AND VARIABLES initial_investment, strategy and months*/
std::tuple<float, int, std::string> startgame() {
    std::cout << "Welcome to Stock Shock. Today is 1st of January of 2023. Let's test your investment skills.\n";
    std::cout << "You will have a series of decisions to make which will affect how your money behaves, so choose wisely!\n";

    // initial investment
    float initial_investment = 20000;
    std::cout << "\nFirst off, how much money would you like to invest? (Enter a positive number or type 'you choose'): ";
    std::string input;
    getline(std::cin, input);
    if (input != "you choose") {
        try {
            initial_investment = std::stof(input);
            if (initial_investment <= 0) throw std::invalid_argument("Must be positive");
        } catch (std::exception&) {
            std::cout << "Invalid input. Using default of 20,000 euros." << std::endl;
            initial_investment = 20000;
        }
    }

    // number of months
    int months = 12;
    std::cout << "\nHow many months would you like to test? (1-12 or type 'you choose'): ";
    getline(std::cin, input);
    if (input != "you choose") {
        try {
            months = std::stoi(input);
            if (months < 1 || months > 12) throw std::out_of_range("Must be between 1 and 12");
        } catch (std::exception&) {
            std::cout << "Invalid input. Using default of 12 months." << std::endl;
            months = 12;
        }
    }

    // investment strategy
    std::string strategy = "Neutral";
    std::cout << "\nWhat investment strategy would you like to use? (Optimistic, Neutral, Conservative, or type 'you choose'):\n";
    std::cout << "Optimistic: Go big or go home! High-stakes investing with volatile stocks and emerging markets.\n";
    std::cout << "Neutral: A balanced strategy with growth stocks and blue-chip names for steady growth.\n";
    std::cout << "Conservative: Low-volatility stocks and steady dividends, focusing on stable companies.\n";
    getline(std::cin, input);
    std::transform(input.begin(), input.end(), input.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    if (input == "you choose" || (input != "optimistic" && input != "conservative")) {
        strategy = "neutral";
    } else if (input == "optimistic") {
        strategy = "optimistic";
    } else if (input == "conservative") {
        strategy = "conservative";
    }

    return std::make_tuple(initial_investment, months, strategy);
}
// Function to create the portfolio
std::map<std::string, double> create_portfolio(const std::vector<std::string>& tickers, double initial_investment) {
    std::map<std::string, double> my_portfolio;
    if (tickers.empty()) {
        std::cerr << "Error: No tickers provided.\n";
        return my_portfolio;
    }
    double price_per_ticker = initial_investment / tickers.size();
    for (const auto& ticker : tickers) {
        my_portfolio[ticker] = price_per_ticker;
    }

    return my_portfolio;
}

// Template function to print a map
template <typename K, typename V>
void print_map(const std::map<K, V>& m, const std::string& map_name = "Map") {
    std::cout << map_name << ":\n";
    for (const auto& pair : m) {
        std::cout << "  " << pair.first << " : " << pair.second << "\n";
    }
}

//Function with the main code testing each portion of the volatility calculation
int main() {
    /*
        std::map<std::string, double> my_portfolio = {
        {"AAPL", 10000},
        {"GOOGL", 15000},
        {"MSFT", 20000},
        {"AMZN", 12000},
        {"TSLA", 18000},
        {"META", 14000},
        {"NFLX", 16000},
        {"NVDA", 11000},
        {"ADBE", 13000},
        {"INTC", 12500}
    };
    */

    //INIT GAME
    float initial_investment;
    int months;
    std::string strategy;
    std::tie(initial_investment, months, strategy) = startgame();
    std::cout << "Investment: " << initial_investment << " Euros, Duration: " << months << " months, Strategy: " << strategy << std::endl;
    
    // GET PRICE PER HOUR
    // Map to store prices for each ticker
    std::map<std::string, std::vector<long double>> ticker_to_prices;
    // List of tickers
    std::vector<std::string> tickers = {
        "NVDA", "AAPL", "MSFT", "AMZN", "GOOGL",
        "META", "TSLA", "TSM", "AVGO", "ORCL"
    };
    // Fetch data for each ticker
    for (const auto& ticker : tickers) {
        get_stock_data(ticker, "2024-01-01", "2024-11-18", ticker_to_prices);
    }
    // Determine the maximum number of hours
    size_t max_hours = 0;
    for (const auto& pair : ticker_to_prices) {
        max_hours = std::max(max_hours, pair.second.size());
    }

    // Determine initial investment per stock
    std::map<std::string,double> my_portfolio = create_portfolio(tickers, initial_investment);
    print_map(my_portfolio, "My Portfolio");

    // START THE FOR LOOP HERE
    // Loop through each hour and create the hourly map
    for (size_t hour = 7; hour < max_hours; ++hour) {
        std::map<std::string, long double> hourly_map;
        for (const auto& pair : ticker_to_prices) {
            const std::string& ticker = pair.first;
            const std::vector<long double>& prices = pair.second;
            if (hour < prices.size()) {
                hourly_map[ticker] = prices[hour];
            }
        }
        print_map(hourly_map, "My hourly_map");

        // GET VOLATILITY MAP
        std::map<std::string, long double> output = ticker_to_vol_hourly(ticker_to_prices);
        std::map<std::string, long double> true_vol = true_volatility(ticker_to_prices, output);
        std::cout << "Volatility map: " << std::endl;
        for (const auto& pair : true_vol) {
            std::cout << pair.first << " : " << pair.second << std::endl;
        }

        // Call the function
        StockManagerResult result = StockManager(true_vol, my_portfolio, strategy);

        // Output results from StockManager
        std::cout << "Buying stocks:\n";
        if (result.buying_stocks.empty()) {
            std::cout << "No stocks to buy.\n";
        } else {
            for (const auto& stock : result.buying_stocks) {
                std::cout << "- " << stock << "\n";
            }
        }

        std::cout << "\nSelling stocks:\n";
        if (result.selling_stocks.empty()) {
            std::cout << "No stocks to sell.\n";
        } else {
            for (const auto& stock : result.selling_stocks) {
                std::cout << "- " << stock << "\n";
            }
        }

        std::cout << "\nReallocation funds: $" << result.reallocation_funds << "\n";

        // Allocate reallocation funds using portfolio_manager
        portfolio_manager(result.buying_stocks, result.reallocation_funds, my_portfolio, strategy, true_vol);

        // Output the final portfolio after allocation
        std::cout << "\nFinal portfolio:\n";
        for (const auto& [stock, value] : my_portfolio) {
            std::cout << stock << ": $" << value << "\n";
        }
    }
    // Inline loop to print the portfolio
    std::cout << "My Portfolio:\n";
    for (const auto& pair : my_portfolio) {
        std::cout << "  " << pair.first << " : " << pair.second << "\n";
    }
    std::cout << "-----------------------------------\n";

    return 0;
}
