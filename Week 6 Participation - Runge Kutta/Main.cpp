using namespace std;

#include<cmath>
#include<algorithm>
#include<numbers>
#include "header.hpp"
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

RK4::RK4(const double dt_in, const double t0_in, const double tmax_in, vector<double> x0_in, vector<double> velocity_in)
    : dt(dt_in), t0(t0_in), t_max(tmax_in), x(9), x0(x0_in), velocity(velocity_in), xdot1(9), xdot2(9), xdot3(9), xdot4(9), phi(x[0]), theta(x[1]), psi(x[2]), DCM({{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}})
{
    // State Vector
    for (int a = 0; a < 3; a++)
    {
        x[a] = x0[a];
        x[a + 3] = velocity[a];
        x[a + 6] = 0;
    }
}

array<array<double, 9>, 2> RK4::derivative(const vector<double> x, const array<array<double, 3>, 3> DCM, const vector<double> velocity, const double t)
{
    array<array<double, 9>, 2> result;
    double phi = x[0];
    double theta = x[1];
    double psi = x[2];
    vector<double> pqr = function(t);
    vector<double> qdot;

    // Gimbal equation
    array<array<double, 3>, 3> Rate = {{{0, -pqr[2], pqr[1]}, {pqr[2], 0, -pqr[0]}, {-pqr[1], pqr[0], 0}}};
    qdot = MatrixMultiplication(Rate, pqr);

    // Strapdown equation
    array<array<double, 3>, 3> Matrix = {{{1, tan(theta)*sin(phi), tan(theta)*cos(phi)}, {0, cos(phi), -sin(phi)}, {0, sin(phi)/cos(theta), cos(phi)/cos(theta)}}};
    array<array<double, 3>, 3> DCMdot = MatrixMultiplication(DCM, Matrix);

    // Obtaining as a vector for output
    int iterator = 0;
    for (int b = 0; b < 3; b++)
    {
        for (int c = 0; c < 3; c++)
        {
            result[1][iterator] = DCMdot[b][c];
            iterator++;
        }
    }

    vector<double> xdot_NED = MatrixMultiplication(DCM, velocity);
    for (int d = 0; d < 3; d++)
    {
        result[0][d] = qdot[d];
        result[0][d + 3] = 0;
        result[0][d + 6] = xdot_NED[d];
    }
    return result;
}

vector<double> RK4::function(double t)
{
    double p, q, r;
    p = pi / 6.0;
    q = cos((t * 6.0) / pi);
    r = 3 * sin((t * 30.0) / pi);
    return {p, q, r};
}

void RK4::new_dt(const double dt_new)
{
    dt = dt_new;
}

vector<vector<double>> RK4::Xdata()
{
    return xhist;
}

vector<vector<double>> RK4::Xdotdata()
{
    return xdothist;
}

vector<double> RK4::time_vector()
{
    vector<double> result;
    for (double e = 0; e < t_max; e += dt)
    {
        result[e] = dt;
    }
    return result;
}

void RK4::run()
{
    int counter = 0;
    for(double t = t0; t < t_max; t += dt)
    {
        State = derivative(x, DCM, velocity, t);
        xdot1 = getxdot(State);
        ydot1 = getydot(State);
        State = derivative(MatrixAddition(x, MatrixMultiplication(xdot1, dt / 2)), MatrixAddition(DCM, MatrixMultiplication(ydot1, dt / 2)), velocity, t + dt);
        xdot2 = getxdot(State);
        ydot2 = getydot(State);
        State = derivative(MatrixAddition(x, MatrixMultiplication(xdot2, dt)), MatrixAddition(DCM, MatrixMultiplication(ydot2, dt)), velocity, t + dt);
        xdot3 = getxdot(State);
        ydot3 = getydot(State);
        State = derivative(MatrixAddition(x, MatrixMultiplication(xdot3, dt)), MatrixAddition(DCM, MatrixMultiplication(ydot3, dt)), velocity, t + dt);
        xdot4 = getxdot(State);
        ydot4 = getydot(State);
        xdothist[counter] = MatrixMultiplication(MatrixAddition(MatrixAddition(xdot1, MatrixMultiplication(xdot2, 2)), MatrixAddition(xdot4, MatrixMultiplication(xdot3, 2))), (1.0 / 6.0));
        xhist[counter + 1] = MatrixAddition(xhist[counter], MatrixMultiplication(xdothist[counter], dt));
        x = xhist[counter + 1];
        ydot_tot = MatrixMultiplication(MatrixAddition(MatrixAddition(ydot1, MatrixMultiplication(ydot2, 2)), MatrixAddition(ydot4, MatrixMultiplication(ydot3, 2))), (1.0 / 6.0));
        DCM = MatrixAddition(DCM, MatrixMultiplication(ydot_tot, dt));
        counter++;
    }
}

// 3x3 times 3x1 matric multiplication
vector<double> MatrixMultiplication(const array<array<double, 3>, 3> arr1, const vector<double> pqr_in)
{
	vector<double> vector_result(3);
	vector_result[0] = (arr1[0][0] * pqr_in[0]) + (arr1[0][1] * pqr_in[1]) + (arr1[0][2] * pqr_in[2]);
	vector_result[1] = (arr1[1][0] * pqr_in[0]) + (arr1[1][1] * pqr_in[1]) + (arr1[1][2] * pqr_in[2]);
	vector_result[2] = (arr1[2][0] * pqr_in[0]) + (arr1[2][1] * pqr_in[1]) + (arr1[2][2] * pqr_in[2]);
	return vector_result;
}

// 1x9 vector times a scalar multiplication
vector<double> MatrixMultiplication(const vector<double> vector, const double scalar)
{
	std::vector<double> result(9);
	for (int f = 0; f < vector.size(); f++)
	{
		result[f] = vector[f] * scalar;
	}
	return result;
}

// 3x3 times 3x3 matrix multiplication
array<array<double, 3>, 3> MatrixMultiplication(const array<array<double, 3>, 3> arr1, const array<array<double, 3>, 3> arr2)
{
	array<array<double, 3>, 3> arr_result;
	for (int g = 0; g < 3; g++) 
    {         
		for (int h = 0; h < 3; h++) 
        {     
			for (int i = 0; i < 3; i++) 
            { 
				arr_result[g][h] = arr_result[g][i] * arr_result[i][h];
			}
		}
	}
	return arr_result;
}

// 3x3 times a scalar matrix multiplication
array<array<double, 3>, 3> MatrixMultiplication(const array<array<double, 3>, 3> arr1, const double scalar)
{
	array<array<double, 3>, 3> arr_result;
	for (int j = 0; j < 3; j++) {
		for (int k = 0; k < 3; k++) {
			arr_result[j][k] = arr1[j][k] * scalar;
		}
	}
	return arr_result;
}

// 3x3 plus 3x3 matrix addition
array<array<double, 3>, 3> MatrixAddition(const array<array<double, 3>, 3> arr1, const array<array<double, 3>, 3> arr2)
{
	array<array<double, 3>, 3> result;
	for (int l = 0; l < 3; l++)
	{
		for (int m = 0; m < 3; m++)
		{
			result[l][m] = arr1[l][m] + arr2[l][m];
		}
	}
	return result;
}

// Vector Addition
vector<double> MatrixAddition(const vector<double> vector1, const vector<double> vector2)
{
	std::vector<double> result(vector1.size());
	if (vector1.size() != vector2.size())
	{
		cout << "Error: both vectors must be the same size to add them together" << endl;
	}
	for (int n = 0; n < vector1.size(); n++)
	{
		result[n] = vector1[n] + vector2[n];
	}
	return result;
}

// Recreates DCM
array<array<double, 3>, 3> getydot(array<array<double, 9>, 2> input)
{
	array<array<double, 3>, 3> new_DCM;
	int iterator = 0;
	for (int o = 0; o < 3; o++) {
		for (int p = 0; p < 3; p++) {
			new_DCM[o][p] = input[1][iterator];
			iterator++;
		}
	}
	return new_DCM;
}

// Creates new State vector
vector<double> getxdot(array<array<double, 9>, 2> input)
{
	vector<double> new_State(9);
	int iterator = 0;
	for (int q = 0; q < 9; q++) {
		new_State[q] = input[0][q];
	}
	return new_State;
}

int main()
{
	double dt = 0.2; // Time integration (0.2, 0.1, 0.025, 0.0125)
	vector<double> x0 = { 0, 0, 0 }; // Initial angles
	vector<double> velocity = { 60 * 6076 / 3600, 0, 0 }; // Speed of the aircraft
	const double t0 = 0.0;
	const double t_max = 60.0;

	// Object for integration
	RK4 NumericalIterator(dt, t0, t_max, x0, velocity);

	// Compute the integration
	NumericalIterator.run();
	vector<double> time_v = NumericalIterator.time_vector();
	vector<vector<double>> X_totData = NumericalIterator.Xdata();
	vector<vector<double>> Xdot_totData = NumericalIterator.Xdotdata();

	// Obtain pqr coordinates
	vector<double> p_coord(time_v.size()), q_coord(time_v.size()), r_coord(time_v.size()), functionresult(3);
	for (int r = 0; r < time_v.size(); r++)
	{
		functionresult = NumericalIterator.function(time_v[r]);
		p_coord[r] = functionresult[0];
		q_coord[r] = functionresult[1];
		r_coord[r] = functionresult[2];
	}

	// Obtain rate of change for phi, theta, and psi
	vector<double> phi_dot(time_v.size()), theta_dot(time_v.size()), psi_dot(time_v.size());
	for (int s = 0; s < time_v.size(); s++)
	{
		phi_dot[s] = Xdot_totData[s][0];
		theta_dot[s] = Xdot_totData[s][1];
		psi_dot[s] = Xdot_totData[s][2];
	}

	// Obtain phi, theta, and psi
	vector<double> phi(time_v.size()), theta(time_v.size()), psi(time_v.size());
	for (int t = 0; t < time_v.size(); t++)
	{
		phi[t] = X_totData[t][0];
        double pi = 3.1459;
		while (phi[t] > pi) 
        {
			phi[t] -= 2.0 * pi;
		}
		theta[t] = X_totData[t][1];
		psi[t] = X_totData[t][2];
	}

	// Obtain Velocity components
	vector<double> velo_1(time_v.size()), velo_2(time_v.size()), velo_3(time_v.size()), velo_mag(time_v.size());
	for (int u = 0; u < time_v.size(); u++)
	{
		velo_1[u] = Xdot_totData[u][6];
		velo_2[u] = Xdot_totData[u][7];
		velo_3[u] = Xdot_totData[u][8];
		velo_mag[u] = sqrt((velo_1[u] * velo_1[u]) + (velo_2[u] * velo_2[u]) + (velo_3[u] * velo_3[u]));
	}

	// Obtain Position components
	vector<double> x_1(time_v.size()), x_2(time_v.size()), x_3(time_v.size()), x_mag(time_v.size());
	for (int v = 0; v < time_v.size(); v++)
	{
		x_1[v] = X_totData[v][6];
		x_2[v] = X_totData[v][7];
		x_3[v] = X_totData[v][8];
		x_mag[v] = sqrt((x_1[v] * x_1[v]) + (x_2[v] * x_2[v]) + (x_3[v] * x_3[v]));
	}

	// Output for Plotting in Excel
	for (int w = 0; w < time_v.size(); w++)
	{
		// Uncommment each line to get its data
		//cout << time_v[w] << "\t" << p_coord[w] << "\t" << q_coord[w] << "\t" << r_coord[w] << endl; // Angular velocity
		//cout << time_v[w] << "\t" << phi_dot[w] << "\t" << theta_dot[w] << "\t" << psi_dot[w] << endl; // Euler angle rates
		//cout << time_v[w] << "\t" << phi[w] << "\t" << theta[w] << "\t" << psi[w] << endl; // Euler angles
		cout << time_v[w] << "\t" << velo_1[w] << "\t" << velo_2[w] << "\t" << velo_3[w] << "\t" << velo_mag[w] << endl; // Velocities
		//cout << time_v[w] << "\t" << x_1[w] << "\t" << x_2[w] << "\t" << x_3[w] << "\t" << x_mag[w] << endl; // Position

	}
	cout << "COPY AND PASTE THE ABOVE DATA INTO EXCEL (TAB DELIMITER)" << endl;
	return 0;
}