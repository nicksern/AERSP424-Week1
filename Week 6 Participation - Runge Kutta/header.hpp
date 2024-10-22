// Header for Matrix Math
using namespace std;
#include<iostream>
#include<vector>
#include<array>

#pragma once

vector<double> MatrixMultiplication(const array<array<double, 3>, 3> arr1, const vector<double> pqr_in);
vector<double> MatrixMultiplication(const vector<double> vec1, const double scalar);
array<array<double, 3>, 3> MatrixMultiplication(const array<array<double, 3>, 3> arr2, const array<array<double, 3>, 3> arr3);
array<array<double, 3>, 3> MatrixMultiplication(const array<array<double, 3>, 3> arr1, const double scalar);
array<array<double, 3>, 3> MatrixAddition(const array<array<double, 3>, 3> arr2, const array<array<double, 3>, 3> arr3);
vector<double> MatrixAddition(const vector<double> vec2, const vector<double> vec3);
array<array<double, 3>, 3> getydot(array<array<double, 9>, 2> input);
vector<double> getxdot(array<array<double, 9>, 2> input);