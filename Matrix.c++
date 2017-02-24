#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Matrix.h"

using namespace std;

void matrix_read(ifstream &ifile, int &op, vector<vector<vector<int>>> &matrices) {
	string op_string, line;
	ifile >> op_string;
	string op_arr[] = {"ef", "rref"};
	op = get_op_num(op_string, op_arr);

	int mat_num = -1;
	int elem;
	while(getline(ifile, line)) {
		if(line.find_first_not_of(' ') == string::npos) {
			matrices.push_back({});
			mat_num++;
			getline(ifile, line);
		}
		vector<int> row;
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

void matrix_eval(int &op, vector<vector<vector<int>>> &matrices) {
	return;
}

void matrix_print(ofstream &o, vector<vector<vector<int>>> matrices) {
	for(int i = 0; i < matrices.size(); i++) {
		for(int j = 0; j < matrices.at(i).size(); j++) {
			for(int k = 0; k < matrices.at(i).at(j).size(); k++) {
				o << matrices.at(i).at(j).at(k) << " ";
			}
		o << "\n";
		}
	o << "\n";
	}
}

void matrix_solve(ifstream &i, ofstream &o) {
	vector<vector<vector<int> > > matrices;
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
