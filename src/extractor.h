
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <ctime>
#include <iomanip> // For std::setprecision and std::fixed


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
                    std::map<std::string, std::vector<double>>& ticker_to_prices) {
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
                    ticker_to_prices[ticker].push_back(static_cast<double>(prices[i]));
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
void save_to_csv(const std::string& filename, const std::map<std::string, std::vector<double>>& ticker_to_prices) {
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