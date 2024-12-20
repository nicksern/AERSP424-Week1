using namespace std;
#include<iostream>
#include<vector>
#include<array>
#ifndef RK4_H
#define RK4_H

class RK4
{
    private:
        // Variables for distance, velocity, angles
        const double pi = 3.1459;
        vector<double> x0;
        vector<double> x;
        vector<double> velocity;
        double theta, phi, psi;
        array<array<double, 3>, 3> DCM;

        // Integration Variables
        const double t_max, t0;
        double dt;
        vector<double> result;

        // Data Variables
        array<array<double, 9>, 2> State;
        array<array<double, 3>, 3> ydot1, ydot2, ydot3, ydot4;
        vector<double> xdot1, xdot2, xdot3, xdot4;
        array<array<double, 3>, 3> ydot_tot;
        const int iter_max = (int)((t_max - t0) / dt) + 2;
        vector<vector<double>> xhist;
        vector<vector<double>> xdothist;

    public:
        vector<double> function(double t);
        void new_dt(const double dt_new);
        array<array<double, 9>, 2> derivative(const vector<double> x, const array<array<double, 3>, 3> DCM, const vector<double> velocity, const double t);
        RK4(const double dt_in, const double tmax_in, const double t0_in, vector<double> x0_in, vector<double> velocity_in);
        vector<vector<double>> Xdata();
        vector<vector<double>> Xdotdata();
        vector<double> time_vector();
        void run();
};
#endif