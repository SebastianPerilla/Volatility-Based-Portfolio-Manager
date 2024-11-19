#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <Eigen/Dense> // For linear algebra

// Struct to hold CSV data
struct StockData {
    std::string ticker;
    std::string timestamp;
    double price;
    int volume;
};

// Function to read CSV
std::vector<StockData> readCSV(const std::string &filename) {
    std::vector<StockData> data;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        StockData row;
        sscanf(line.c_str(), "%[^,],%[^,],%lf,%d",
               &row.ticker[0], &row.timestamp[0], &row.price, &row.volume);
        data.push_back(row);
    }
    return data;
}

// Compute log returns
std::vector<double> computeLogReturns(const std::vector<StockData> &data) {
    std::vector<double> logReturns;
    for (size_t i = 1; i < data.size(); ++i) {
        double logReturn = std::log(data[i].price / data[i - 1].price);
        logReturns.push_back(logReturn);
    }
    return logReturns;
}

// MCMC sampling function
void runMCMC(const std::vector<double> &logReturns, size_t iterations, double priorMean, double priorVariance) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> proposalDist(0, 1); // Standard Gaussian for proposal

    double currentMean = priorMean;
    double currentVariance = priorVariance;
    std::vector<double> sampledMeans, sampledVariances;

    for (size_t i = 0; i < iterations; ++i) {
        double proposedMean = currentMean + proposalDist(gen); // Propose a new mean
        double proposedVariance = std::fabs(currentVariance + proposalDist(gen)); // Variance must be > 0

        // Compute log-likelihood and prior
        double logLikelihoodCurrent = 0.0, logLikelihoodProposed = 0.0;
        for (double r : logReturns) {
            logLikelihoodCurrent += -0.5 * std::pow(r - currentMean, 2) / currentVariance;
            logLikelihoodProposed += -0.5 * std::pow(r - proposedMean, 2) / proposedVariance;
        }

        double acceptanceRatio = std::exp(logLikelihoodProposed - logLikelihoodCurrent);
        if (std::uniform_real_distribution<>(0, 1)(gen) < acceptanceRatio) {
            currentMean = proposedMean;
            currentVariance = proposedVariance;
        }

        sampledMeans.push_back(currentMean);
        sampledVariances.push_back(currentVariance);
    }

    // Print results
    std::cout << "Sampled Means and Variances:\n";
    for (size_t i = 0; i < sampledMeans.size(); ++i) {
        std::cout << sampledMeans[i] << ", " << sampledVariances[i] << "\n";
    }
}

int main() {
    // Read data
    auto data = readCSV("data.csv");
    auto logReturns = computeLogReturns(data);

    // Run MCMC
    runMCMC(logReturns, 1000, 0.0, 1.0);

    return 0;
}
