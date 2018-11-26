#include <iostream>
#include <random>
#include <chrono>

void gradient_descent(const double x[], const double y[], const size_t& epoches, const size_t& N, const double& learn_rate, double& m, double& b) {
    double p;
    double err;
    size_t idx;
    for(size_t i = 0; i < epoches * N; i++) {
        idx = i % N;
        p = b + m * x[idx];
        err = p - y[idx];
        b = b - learn_rate * err;
        m = m - learn_rate * err * x[idx];
    }
}
int main(int argc, char* argv[]) {
    size_t N;
    double learn_rate;
    size_t epoches;
    double correct_b = 1.0;
    double correct_m = 0.5;
    if (argc != 4) {
        N = 1000;
        learn_rate = 1.0e-3;
        epoches = 5;
    } else {
        N = std::strtoul(argv[1], NULL, 10); 
        learn_rate = std::strtod(argv[2], NULL); 
        epoches = std::strtoul(argv[3], NULL,10); 
        std::cout << "N = " << N << ", learn_rate = " << learn_rate << ", epoches = " << epoches << std::endl;
    }
    std::chrono::steady_clock::time_point tStart, tStop;
    tStart = std::chrono::steady_clock::now();
    double* m_real = new double[N];
    double* b_real = new double[N];
    double* x = new double[N];
    double* y = new double[N];
    // setting up random generators
    std::default_random_engine generator;
    std::normal_distribution<double> m_dist(correct_m,0.2);
    std::normal_distribution<double> b_dist(correct_b,0.2);
    std::normal_distribution<double> x_dist(0.0,1);

    // creating random dataset
    for(size_t i = 0; i < N; i++) {
        m_real[i] = m_dist(generator);
        b_real[i] = b_dist(generator);
        x[i] = x_dist(generator);
        y[i] = m_real[i] * x[i] + b_real[i];
    }
    // estimated b, m
    double b = 0;
    double m = 0;
    // gradient descent 
    gradient_descent(x, y, epoches, N, learn_rate, m, b);
    tStop = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tStop - tStart);
    std::cout << "Execution Time: " << ms.count() << " milliseconds" << std::endl;
    std::cout << "Predicted:\nb = " << b  << ", m = " << m << std::endl;
    std::cout << "Correct:\nb = " << correct_b  << ", m = " << correct_m << std::endl;
    std::cout << std::endl;
    delete[] m_real;
    delete[] b_real;
    delete[] x;
    delete[] y;
    return 0;
}
