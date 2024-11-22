#include "volatility_formula.h"
#include "portfolio_manager.h"
#include "stock_manager.h"
#include "volatility_parse.h"
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
#include <fstream>
#include <sstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <ctime>
#include <iomanip> // For std::setprecision and std::fixed



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



//ISMAS
using json = nlohmann::json;

// Callback function for CURL to write response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Convert dates to Unix timestamps
long convert_to_timestamp(const std::string& date) {
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return std::mktime(&tm);
}

// Fetch stock data and store prices in a map
void get_stock_data(const std::string& ticker, const std::string& start_date, const std::string& end_date, 
                    std::map<std::string, std::vector<long double>>& ticker_to_prices) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return;
    }

    long period1 = convert_to_timestamp(start_date);
    long period2 = convert_to_timestamp(end_date);

    std::string url = "https://query1.finance.yahoo.com/v8/finance/chart/" + ticker +
                      "?period1=" + std::to_string(period1) +
                      "&period2=" + std::to_string(period2) +
                      "&interval=1h";

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::string response_data;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Failed to fetch data: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        return;
    }

    try {
        json data = json::parse(response_data);

        if (!data["chart"]["result"][0]["timestamp"].is_null()) {
            auto timestamps = data["chart"]["result"][0]["timestamp"];
            auto prices = data["chart"]["result"][0]["indicators"]["quote"][0]["close"];

            for (size_t i = 0; i < timestamps.size(); i++) {
                if (!prices[i].is_null()) {
                    ticker_to_prices[ticker].push_back(static_cast<long double>(prices[i]));
                }
            }

            std::cout << "Data for " << ticker << " has been processed and stored." << std::endl;
        }
    } catch (const json::exception& e) {
        std::cerr << "JSON error: " << e.what() << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

// Save data to a CSV file
void save_to_csv(const std::string& filename, const std::map<std::string, std::vector<long double>>& ticker_to_prices) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    file << "ticker,price\n";
    file << std::fixed << std::setprecision(20); 
    for (const auto& [ticker, prices] : ticker_to_prices) {
        for (const auto& price : prices) {
            file << ticker << "," << price << "\n";
        }
    }
    file.close();

    std::cout << "Data has been saved to " << filename << std::endl;
}




// Function to print a std::map
void printMap(const std::map<std::string, double>& myMap, const std::string& title) {
    std::cout << title << std::endl;
    for (const auto& [key, value] : myMap) {
        std::cout << key << ": " << value << std::endl;
    }
    std::cout << std::endl;
}


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

    //INIT GAME
    float initial_investment;
    int months;
    std::string strategy;
    std::tie(initial_investment, months, strategy) = startgame();
    std::cout << "Investment: " << initial_investment << " Euros, Duration: " << months << " months, Strategy: " << strategy << std::endl;
    
    //TEST
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

    // Save data to a CSV file
    save_to_csv("stock_data.csv", ticker_to_prices);

    // Print the ticker-to-prices mapping
    std::cout << std::fixed << std::setprecision(20);
    for (const auto& [ticker, prices] : ticker_to_prices) {
        std::cout << "Ticker: " << ticker << "\nPrices: ";
        for (const auto& price : prices) {
            std::cout << price << " ";
        }
        std::cout << "\n";
    }


    // GET VOLATILITY MAP
    std::map<std::string, double> output = ticker_to_vol_hourly(standard_volatility);
    std::map<std::string, double> true_vol = true_volatility(standard_volatility, output);

    // Print the maps
    printMap(output, "Hourly Volatility:");
    printMap(true_vol, "True Volatility:");

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

    return 0;
}