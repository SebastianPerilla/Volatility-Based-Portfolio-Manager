#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <ctime>
#include <sstream>
#include <algorithm>

using json = nlohmann::json;

// Callback function for CURL to write response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Add this function to convert dates to Unix timestamps
long convert_to_timestamp(const std::string& date) {
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return std::mktime(&tm);
}

// Add this new function for timestamp conversion
std::string format_timestamp(long unix_timestamp) {
    std::time_t time = unix_timestamp;
    std::tm* tm = std::localtime(&time);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S-05:00", tm);
    return std::string(buffer);
}

// Add this struct before get_stock_data function
struct StockRecord {
    std::string ticker;
    std::string timestamp;
    double price;
    long volume;
};

// Function to get stock data
void get_stock_data(std::vector<StockRecord>& all_records, const std::string& ticker, const std::string& start_date, const std::string& end_date) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return;
    }

    // Convert dates to timestamps
    long period1 = convert_to_timestamp(start_date);
    long period2 = convert_to_timestamp(end_date);

    std::string url = "https://query1.finance.yahoo.com/v8/finance/chart/" + ticker +
                      "?period1=" + std::to_string(period1) +
                      "&period2=" + std::to_string(period2) +
                      "&interval=1h";

    // Add a user agent to avoid 403 errors
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
        std::cout << "Response size: " << response_data.length() << std::endl;
        std::cout << "First 200 characters of response: " << response_data.substr(0, 200) << std::endl;
        json data = json::parse(response_data);

        // Get timezone and GMT offset from the response
        std::string timezone = data["chart"]["result"][0]["meta"]["timezone"];
        int gmtOffset = data["chart"]["result"][0]["meta"]["gmtoffset"];

        // Update format_timestamp to use the GMT offset
        auto format_with_tz = [](long unix_timestamp) {
            std::time_t time = unix_timestamp;
            std::tm* tm = std::localtime(&time);
            char buffer[32];

            // Check if date is in DST
            bool is_dst = tm->tm_isdst > 0;

            // EST is -5:00, EDT is -4:00
            std::string tz_str = is_dst ? "-04:00" : "-05:00";

            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
            return std::string(buffer) + tz_str;
        };

        // Instead of writing directly to file, store records in memory
        if (!data["chart"]["result"][0]["timestamp"].is_null()) {
            auto timestamps = data["chart"]["result"][0]["timestamp"];
            auto prices = data["chart"]["result"][0]["indicators"]["quote"][0]["close"];
            auto volumes = data["chart"]["result"][0]["indicators"]["quote"][0]["volume"];

            for (size_t i = 0; i < timestamps.size(); i++) {
                if (!prices[i].is_null() && !volumes[i].is_null()) {
                    StockRecord record{
                        ticker,
                        format_with_tz(timestamps[i]),
                        prices[i],
                        volumes[i]
                    };
                    all_records.push_back(record);
                }
            }

            std::cout << "Data for " << ticker << " has been collected" << std::endl;
        }
    } catch (const json::exception& e) {
        std::cerr << "JSON error: " << e.what() << std::endl;
        std::cerr << "Response received: " << response_data.substr(0, 100) << "..." << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

int main() {
    std::vector<StockRecord> all_records;

    // List of tickers
    std::vector<std::string> tickers = {
        "NVDA", "AAPL", "MSFT", "AMZN", "GOOGL",
        "META", "TSLA", "TSM", "AVGO", "ORCL"
    };

    // Collect data for each ticker
    for (const auto& ticker : tickers) {
        get_stock_data(all_records, ticker, "2024-01-01", "2024-11-18");
    }

    // Sort records by timestamp
    std::sort(all_records.begin(), all_records.end(),
        [](const StockRecord& a, const StockRecord& b) {
            return a.timestamp < b.timestamp;
        });

    // Write sorted records to file
    std::ofstream file("stock_data.csv");
    file << "ticker,timestamp,price,volume\n";

    for (const auto& record : all_records) {
        file << record.ticker << ","
             << record.timestamp << ","
             << record.price << ","
             << record.volume << "\n";
    }

    file.close();
    return 0;
}
