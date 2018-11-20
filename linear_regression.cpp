#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[]) {
    size_t N;
    double learn_rate;
    size_t epoches;
    if (argc != 4) {
        N = 1000;
        learn_rate = 1.0e-3;
        epoches = 5;
    } else {
        N = std::strtoul(argv[1], NULL, 10); 
        learn_rate = std::strtod(argv[2], NULL); 
        epoches = std::strtoul(argv[3], NULL,10); 
        std::cout << "N = " << N << ", learn_rate = " << learn_rate << ", epoches" << epoches << std::endl;
    }
    std::vector<double> m_real(N), b_real(N), x(N), y(N);
    std::default_random_engine generator;
    std::normal_distribution<double> m_dist(0.5,0.2);
    std::normal_distribution<double> b_dist(1.0,0.2);
    std::normal_distribution<double> x_dist(0.0,1);
    for(size_t i = 0; i < N; i++) {
        m_real[i] = m_dist(generator);
        b_real[i] = b_dist(generator);
        x[i] = x_dist(generator);
        y[i] = m_real[i] * x[i] + b_real[i];
    }
    double b = 0;
    double m = 0;
    // gradient descent 
    for(size_t i = 0; i < epoches * N; i++) {
        int idx = i % N;
        double p = b+m * x[idx];
        double err = p - y[idx];
        b = b - learn_rate * err;
        m = m - learn_rate * err * x[idx];
        std::cout << "b = " << b  << ", m = " << m << ", err = " << err << "\n";
    }
    std::cout << std::endl;

    return 0;
}
