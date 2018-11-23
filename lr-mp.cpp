#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <tbb/tbb.h>

template<typename T,typename R, typename C>
class Body{
    T m_acc;
    R* m_coor;
    T* m_out;
    const T m_i;
    const C m_c;

    public:

    Body(T* out,R* co, T i, C c): m_acc(i),m_coor(co), m_out(out), m_i(i), m_c(c) {}
    T get_accumul() const { return m_acc; }

    template<typename Tag>
    void operator()(const tbb::blocked_range<unsigned long>& r, Tag){
        T temp = m_acc;
        for (int i = r.begin(); i != r.end(); i++){
            std::cout << "In () at r of : " << i << std::endl;
            std::cout << m_out << std::endl;
            std::cout << "variables: " << m_out[i].s_b;
            std::cout <<" " << m_out[i].s_m;
            std::cout << " " << m_coor[i].s_x <<std::endl;
            temp = m_c(temp, m_out[i],m_coor[i]);
            if (Tag::is_final_scan())
                m_out[i] = temp;
        }
        m_acc = temp;
    }
    Body(Body& b, tbb::split) : m_acc(b.m_i), m_out(b.m_out), m_i(b.m_i), m_c(b.m_c){}
    void reverse_join(Body& a){ m_acc = (m_acc + a.m_acc)/2; }
    void assign(Body& b) { m_acc = b.m_acc ; }
};
struct Para{
    double s_m;
    double s_b;
    Para(double m, double b):s_m(m),s_b(b){}
    Para():Para(0,0){};
    Para operator+(const Para& a)const{
        return Para(s_m+a.s_m,s_b+a.s_b);
    };
    Para operator/(const double a)const{
        return Para(s_m/2,s_b/2);
    };
    Para operator=(const Para& a){
        s_m = a.s_m;
        s_b = a.s_b;
        return *this;
    };
};
struct Coor{
    double s_x;
    double s_y;
};

template<typename T,typename R, typename C>
T scan( T* out, R* co, int n, T identity, C combine){
    Body<T,R,C> body(out, co, identity, combine);
    tbb::parallel_scan( tbb::blocked_range<unsigned long>(0,n), body );
    return body.get_accumul();
}


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

    // creating random dataset
    Coor* c = new Coor[N]; /* coordinates */
    double m_real[N], b_real[N];
    std::default_random_engine generator;
    std::normal_distribution<double> m_dist(0.5,0.2);
    std::normal_distribution<double> b_dist(1.0,0.2);
    std::normal_distribution<double> x_dist(0.0,1);
    std::cout<<"generating data"<<std::endl;
#pragma omp parallel for
    for(size_t i = 0; i < N; i++) {
        m_real[i] = m_dist(generator);
        b_real[i] = b_dist(generator);
        c[i].s_x = x_dist(generator);
        c[i].s_y = m_real[i] * c[i].s_x + b_real[i];
    }

    std::cout <<"setting up analysis"<<std::endl;
    Para* a = new Para[N]; /* parameters */

    auto calc = [&](Para temp, Para a, Coor c ) -> Para {
        std::cout << "variables: " << temp.s_b <<" " << temp.s_m<< " " << c.s_x <<std::endl;
        double p = temp.s_b + temp.s_m * c.s_x;
        std::cout << "p of the calc"<<std::endl;
        double err = p - c.s_y;
        std::cout << "err of the calc"<<std::endl;
        a.s_b = temp.s_b - learn_rate * err;
        std::cout << "a.s_b of the calc"<<std::endl;
        a.s_m = temp.s_m - learn_rate * err;
        std::cout << "a.s_m of the calc"<<std::endl;
        return a;
    };

    Para final;
    std::cout<<"starting analysis"<<std::endl;
    for(size_t i = 0 ; i < epoches ; i++){
        std::cout << "at : " << i << std::endl;
        final = scan(a,c,N,final,calc);
    }
    std::cout << "b = " << final.s_b  << ", m = " << final.s_m << std::endl;
    delete [] a;
    delete [] c;
    return 0;
}
