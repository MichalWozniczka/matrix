#ifndef Voting_h
#define Voting_h

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void matrix_read(ifstream &ifile, int &op, vector<vector<vector<int>>> &matrices);

int get_op_num(string op_string, string op_arr[]);

void matrix_eval(int &op, vector<vector<vector<int>>> &matrices);

void matrix_print(ofstream &o, vector<vector<vector<int>>> matrices);

void matrix_solve(ifstream &i, ofstream &o);

#endif
