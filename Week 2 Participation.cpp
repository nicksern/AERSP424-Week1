using namespace std;
#include <iostream>
#include <vector>
#include <tuple>
#include <array>

double f(double t, double y)
{
    double f = t + y;
    return f;
}

tuple<double, double, double, double> rk4funct(double t, double y, double d){
    double k1, k2, k3, k4;
    k1 = d * f(t, y);
    k2 = d * f(t + 0.5 * d, y + 0.5 * k1);
    k3 = d * f(t + 0.5 * d, y + 0.5 * k2);
    k4 = d * f(t + d, y + k3);
    return std::make_tuple(k1, k2, k3, k4);
}

int main()
{
    double y0 {0};
    double t0 (0);
    double d {0};
    int n {0};
    double phi, theta, psi;
    vector<double> yvector;
    vector<double> tvector;
    double array[3][3] {
        {1, 0, 0},
        {0, cos(phi), -sin(phi)},
        {0, sin(phi), cos(phi)}
    };

    std::cout << "Please input values for step size, initial y, initial t, and number of iterations";
    std::cin >> d >> y0 >> t0 >> n;
    yvector.push_back(y0);
    tvector.push_back(t0);

    for (int i {0}; i < n; ++i){
        double k1, k2, k3, k4;
        auto ret = rk4funct (tvector[i], yvector[i], d);
        k1 = get<0>(ret);
        k2 = get<1>(ret);
        k3 = get<2>(ret);
        k4 = get<3>(ret);

        yvector[i + 1] = yvector[i] + (d / 6)*(k1 + k2 + k3 + k4);
        tvector[i + 1] = tvector[i] + d;
    }
}