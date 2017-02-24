#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "Matrix.h"

using namespace std;

void matrix_read(ifstream &ifile, int &op, vector<vector<vector<double>>> &matrices) {
	string op_string, line;
	getline(ifile, op_string);
	string op_arr[] = {"rref"};
	op = get_op_num(op_string, op_arr);

	int mat_num = -1;
	int elem;
	while(getline(ifile, line)) {
		if(line.find_first_not_of(' ') == string::npos) {
			matrices.push_back({});
			mat_num++;
			getline(ifile, line);
		}
		vector<double> row;
		stringstream s;
		s << line;
		while(s >> elem) {
			row.push_back(elem);
		}
		matrices.at(mat_num).push_back(row);
	}
}

int get_op_num(string op_string, string op_arr[]) {
	int size = sizeof(op_arr)/sizeof(op_arr[0]);

	for(int i = 0; i < size; i++) {
		if(op_string == op_arr[i]) {
			return i;
		}
	}
	return -1;
}

void row_swap(vector<vector<double>> &matrix, int r1, int r2) {
	vector<double> temp = matrix.at(r1);
	matrix.at(r1) = matrix.at(r2);
	matrix.at(r2) = temp;
}

void row_add(double c, vector<vector<double>> &matrix, int r1, int r2) {
	for(int i = 0; i < matrix.at(r2).size(); i++) {
		matrix.at(r2).at(i) += c * matrix.at(r1).at(i);
	}
}

void row_scale(double c, vector<vector<double>> &matrix, int r) {
	for(int i = 0; i < matrix.at(r).size(); i++) {
		if(matrix.at(r).at(i) != 0) {
			matrix.at(r).at(i) *= c;
		}
	}
}

void rref(vector<vector<double>> &matrix) {
	int x = 0;
	int y = 0;

	while(x != matrix.at(0).size() && y != matrix.size()) {
		while(x != matrix.at(0).size() && matrix.at(y).at(x) == 0) {
			bool swapped = 0;
			for(int i = y+1; i < matrix.size(); i++) {
				if(matrix.at(i).at(x) != 0) {
					row_swap(matrix, i, y);
					swapped = 1;
					break;
				}
			}
			if(swapped == 0) {
				x++;
			}
		}
		if(x >= matrix.at(0).size()) {
			break;
		}
		for(int i = 0; i < matrix.size(); i++) {
			if(i != y) {
				row_add(-(matrix.at(i).at(x)/matrix.at(y).at(x)), matrix, y, i);
			}
		}
		x++;
		y++;
	}
	for(int i = 0; i < matrix.size(); i++) {
		for(int j = 0; j < matrix.at(i).size(); j++) {
			if(matrix.at(i).at(j) != 0) {
				row_scale(1/matrix.at(i).at(j), matrix, i);
				break;
			}
		}
	}
}	

void matrix_eval(int &op, vector<vector<vector<double>>> &matrices) {
	if(op == 0) {
		rref(matrices.at(0));
	}
	return;
}

void matrix_print(ofstream &o, vector<vector<vector<double>>> matrices) {
	for(int i = 0; i < matrices.size(); i++) {
		int max_strlen[matrices.at(i).at(0).size()];
		for(int k = 0; k < matrices.at(i).at(0).size(); k++) {
			for(int j = 0; j < matrices.at(i).size(); j++) {
				string s = to_string(matrices.at(i).at(j).at(k));
				if(s.length() > max_strlen[k]) {
					max_strlen[k] = s.length();
				}
			}
		}
		for(int j = 0; j < matrices.at(i).size(); j++) {
			for(int k = 0; k < matrices.at(i).at(j).size(); k++) {
				string s = to_string(matrices.at(i).at(j).at(k));
				int spaces = max_strlen[k] - s.length();
				for(int l = 0; l < spaces; l++) {
					o << " ";
				}
				o << s << " ";
			}
		o << "\n";
		}
	o << "\n";
	}
}

void matrix_solve(ifstream &i, ofstream &o) {
	vector<vector<vector<double> > > matrices;
	int op;
	matrix_read(i, op, matrices);
	matrix_eval(op, matrices);
	matrix_print(o, matrices);
}

int main() {
	ifstream ifile;
	ofstream ofile;
	ifile.open("input.txt");
	ofile.open("output.txt");
	matrix_solve(ifile, ofile);
	ifile.close();
	ofile.close();
}
