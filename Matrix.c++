#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cassert>

#include "Matrix.h"

using namespace std;

void matrix_read(ifstream &ifile, string &op, vector<matrix> &matrices) {
	string line;
	getline(ifile, op);
	int elem;
	while(getline(ifile, line)) {
		vector<vector<double>> mat;
		while(getline(ifile, line) && line.find_first_not_of(' ') != string::npos) {
			vector<double> row;
			stringstream s;
			s << line;
			while(s >> elem) {
				row.push_back(elem);
			}
			mat.push_back(row);
		}
		matrix mat_obj = matrix(mat);
		matrices.push_back(mat_obj);
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

bool is_num(string s) {
	return s.find_first_not_of("0123456789") == string::npos;
}

bool is_letter(string s) {
	return !is_num(s) && s.length() == 1;
}

void matrix_eval(string &op, vector<matrix> &matrices) {
	list<string> bin_functs = {};
	list<string> un_functs = {"ref", "rref"};
	list<string> expr;

	istringstream s(op);
	string token;
	while(getline(s, token, ' ')) {
		expr.push_back(token);
	}
	list<string>::iterator i;
	i = expr.begin();
	while(expr.begin() != prev(expr.end())) {
		string oper, arg1, arg2;
		if(find(bin_functs.begin(), bin_functs.end(), *i) != bin_functs.end()) {
		}
		else if(find(un_functs.begin(), un_functs.end(), *i) != un_functs.end()) {
			string oper = *i;
			string arg1 = *(prev(i));

			if(oper == "ref" && is_letter(arg1)) {
				i--;
				expr.erase(next(i));
				matrix temp = matrices.at(arg1.c_str()[0]-65);
				temp.ref();
				string new_arg(1, matrices.size()+1);
				*i = new_arg;
				matrices.push_back(temp);
			}
			else if(oper == "rref" && is_letter(arg1)) {
				i--;
				expr.erase(next(i));
				matrix temp = matrices.at(arg1.c_str()[0]-65);
				temp.rref();
				string new_arg(1, matrices.size()+1);
				*i = new_arg;
				matrices.push_back(temp);
			}
		}
		i++;
	}
	
	return;
}

void matrix_print(ofstream &o, vector<matrix> matrices) {
	for(int i = 0; i < matrices.size(); i++) {
		matrices.at(i).print(o);
		o << "\n";
	}
}

void matrix_solve(ifstream &i, ofstream &o) {
	vector<matrix> matrices;
	string op;
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
