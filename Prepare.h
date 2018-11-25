#ifndef RANDOM_H
#define RANDOM_H
#include <random>
#include "Coor.h"
void prepare (double* m, double* b, Coor* c, size_t N){
    std::default_random_engine generator;
    std::normal_distribution<double> m_dist(0.5,0.2);
    std::normal_distribution<double> b_dist(1.0,0.2);
    std::normal_distribution<double> x_dist(0.0,1);
#pragma omp parallel for
    for(size_t i = 0; i < N; i++) {
        m[i] = m_dist(generator);
        b[i] = b_dist(generator);
        c[i].s_x = x_dist(generator);
        c[i].s_y = m[i] * c[i].s_x + b[i];
    }

}
#endif
