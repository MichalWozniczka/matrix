#ifndef Voting_h
#define Voting_h

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void matrix_read(ifstream &ifile, int &op, vector<vector<vector<double>>> &matrices);

int get_op_num(string op_string, string op_arr[]);

void row_swap(vector<vector<double>> &matrix, int r1, int r2);

void row_add(double c, vector<vector<double>> &matrix, int r1, int r2);

void row_scale(double c, vector<vector<double>> &matrix, int r);

void rref(vector<vector<double>> &matrix);

void matrix_eval(int &op, vector<vector<vector<double>>> &matrices);

void matrix_print(ofstream &o, vector<vector<vector<double>>> matrices);

void matrix_solve(ifstream &i, ofstream &o);

#endif
