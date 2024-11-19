#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>

const int niter = 1000000;
const double av_x = -0.0930181e0;
const double av_y = 0.0475899e0;
const double av_xx = 1.06614e0;
const double av_yy = 1.28152e0;
const double av_xy = -0.504944e0;
const int nsample = 100;
const int ndim = 2; // Don't change this parameter.
const double step_A = 0.1e0;
const int nskip = 10;

struct StockData {
    std::vector<double> prices; // Store stock prices for a ticker
};

// Gaussian Random Number Generator with Box Muller Algorithm
int BoxMuller(double& p, double& q) {
    double pi = 2e0 * asin(1e0);
    double r = (double)rand() / RAND_MAX;
    double s = (double)rand() / RAND_MAX;
    p = sqrt(-2e0 * log(r)) * sin(2e0 * pi * s);
    q = sqrt(-2e0 * log(r)) * cos(2e0 * pi * s);
    return 0;
}

// Calculation of the action
double calc_action(const double A[ndim][ndim], const double mu[ndim]) {
    double action = A[0][0] * ((mu[0] - av_x) * (mu[0] - av_x) + av_xx - av_x * av_x)
                    + A[1][1] * ((mu[1] - av_y) * (mu[1] - av_y) + av_yy - av_y * av_y)
                    + A[0][1] * ((mu[0] - av_x) * (mu[1] - av_y) + av_xy - av_x * av_y) * 2e0
                    - log(A[0][0] * A[1][1] - A[0][1] * A[1][0]);

    action = 0.5e0 * action * nsample;

    action = action + 0.5e0 * A[0][0] * A[0][0] + 0.5e0 * A[1][1] * A[1][1] + A[0][1] * A[0][1] + 0.5e0 * mu[0] * mu[0] + 0.5e0 * mu[1] * mu[1];
    return action;
}

void read_csv(const std::string& filename, std::map<std::string, StockData>& stock_data) {
    std::ifstream file(filename);
    std::string line;

    // Skip header line
    std::getline(file, line);

    // Read each line from the CSV file
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string ticker, timestamp;
        double price;
        int volume;

        // Read ticker, timestamp, price, and volume
        std::getline(ss, ticker, ',');
        std::getline(ss, timestamp, ',');
        ss >> price;
        ss.ignore(); // Ignore the comma
        ss >> volume;

        // Store the price for the given ticker
        stock_data[ticker].prices.push_back(price);
    }
}

// Main function to perform MCMC and calculate mean and variance
int main() {
    double mu[ndim];
    double A[ndim][ndim];
    double dx;
    int naccept = 0;
    srand((unsigned)time(NULL));

    // Variables to accumulate sums for mean and variance calculation
    double sum_mu[ndim] = {0.0, 0.0};
    double sum_mu_sq[ndim] = {0.0, 0.0};
    double sum_A[ndim][ndim] = {{0.0, 0.0}, {0.0, 0.0}};
    double sum_A_sq[ndim][ndim] = {{0.0, 0.0}, {0.0, 0.0}};

    // Read stock data from CSV file
    std::map<std::string, StockData> stock_data;
    read_csv("data.csv", stock_data);

    // Loop over each stock (ticker)
    for (const auto& [ticker, data] : stock_data) {
        std::cout << "Processing " << ticker << "...\n";

        // Set initial configuration
        A[0][0] = 1e0;
        A[1][1] = 1e0;
        A[0][1] = 0e0;
        A[1][0] = A[0][1];
        mu[0] = 0e0;
        mu[1] = 0e0;

        // Perform MCMC for this stock
        for (int iter = 0; iter != niter; iter++) {
            double r1, r2;
            BoxMuller(r1, r2);
            mu[0] = r1 / sqrt(1e0 + nsample * A[0][0]);
            mu[0] = mu[0] - nsample / (1e0 + nsample * A[0][0]) * (A[0][1] * (mu[1] - av_y) - A[0][0] * av_x);

            mu[1] = r2 / sqrt(1e0 + nsample * A[1][1]);
            mu[1] = mu[1] - nsample / (1e0 + nsample * A[1][1]) * (A[1][0] * (mu[0] - av_x) - A[1][1] * av_y);

            // Metropolis update for A
            double backup_A[ndim][ndim];
            backup_A[0][0] = A[0][0];
            backup_A[1][1] = A[1][1];
            backup_A[0][1] = A[0][1];
            backup_A[1][0] = A[1][0];

            double action_init = calc_action(A, mu);
            dx = (double)rand() / RAND_MAX - 0.5e0;
            A[0][0] = A[0][0] + dx * step_A * 2e0;
            dx = (double)rand() / RAND_MAX - 0.5e0;
            A[1][1] = A[1][1] + dx * step_A * 2e0;
            dx = (double)rand() / RAND_MAX - 0.5e0;
            A[1][0] = A[1][0] + dx * step_A * 2e0;
            A[0][1] = A[0][1] + dx * step_A * 2e0;
            double action_fin = calc_action(A, mu);

            double metropolis = (double)rand() / RAND_MAX;
            if (exp(action_init - action_fin) > metropolis) {
                naccept++;
            } else {
                A[0][0] = backup_A[0][0];
                A[1][1] = backup_A[1][1];
                A[0][1] = backup_A[0][1];
                A[1][0] = backup_A[1][0];
            }

            // Accumulate sums for mean and variance calculation
            for (int i = 0; i < ndim; ++i) {
                sum_mu[i] += mu[i];
                sum_mu_sq[i] += mu[i] * mu[i];
                for (int j = 0; j < ndim; ++j) {
                    sum_A[i][j] += A[i][j];
                    sum_A_sq[i][j] += A[i][j] * A[i][j];
                }
            }

            if ((iter + 1) % nskip == 0) {
                std::cout << std::fixed << std::setprecision(6)
                          << A[0][0] << "  "
                          << A[1][1] << "  "
                          << A[0][1] << "  "
                          << mu[0] << "  "
                          << mu[1] << "  "
                          << (double)naccept / (iter + 1) << std::endl;
            }
        }

        // Calculate final mean and variance for this stock
        double mean_mu[ndim];
        double var_mu[ndim];
        double mean_A[ndim][ndim];
        double var_A[ndim][ndim];

        for (int i = 0; i < ndim; ++i) {
            mean_mu[i] = sum_mu[i] / niter;
            var_mu[i] = (sum_mu_sq[i] / niter) - (mean_mu[i] * mean_mu[i]);
        }

        for (int i = 0; i < ndim; ++i) {
            for (int j = 0; j < ndim; ++j) {
                mean_A[i][j] = sum_A[i][j] / niter;
                var_A[i][j] = (sum_A_sq[i][j] / niter) - (mean_A[i][j] * mean_A[i][j]);
            }
        }

        // Output the mean and variance
        std::cout << "Stock: " << ticker << "\n";
        std::cout << "Mean Mu: [" << mean_mu[0] << ", " << mean_mu[1] << "]\n";
        std::cout << "Variance Mu: [" << var_mu[0] << ", " << var_mu[1] << "]\n";
        std::cout << "Mean A: [[" << mean_A[0][0] << ", " << mean_A[0][1] << "], ["
                  << mean_A[1][0] << ", " << mean_A[1][1] << "]]\n";
        std::cout << "Variance A: [[" << var_A[0][0] << ", " << var_A[0][1] << "], ["
                  << var_A[1][0] << ", " << var_A[1][1] << "]]\n";
    }

    return 0;
}
