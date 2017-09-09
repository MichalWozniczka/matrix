#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cassert>
#include <functional>

#include "Matrix.h"

using namespace std;

void matrix_read(ifstream &ifile, string &op, vector<matrix> &matrices) {
	string line;
	getline(ifile, op);
	double elem;

	getline(ifile, line);
	while(getline(ifile, line)) {
		vector<vector<double>> mat;
		while(line.find_first_not_of(' ') != string::npos) {
			vector<double> row;
			stringstream s;
			s << line;
			while(s >> elem) {
				row.push_back(elem);
			}
			mat.push_back(row);
			if(!getline(ifile, line)) {
				break;
			}
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
	return s.find_first_not_of("-.0123456789") == string::npos;
}

bool is_letter(string s) {
	return !is_num(s) && s.length() == 1;
}

string matrix_eval(string &op, vector<matrix> &matrices) {
	list<string> bin_functs = {"*", "+", "-", "^", "<-", "."};
	list<string> un_functs = {"u", "l", "ref", "rref", "det", "eval", "gram", "q", "r", "||"};
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
		matrix temp;
		bool var = 1;
		if(find(bin_functs.begin(), bin_functs.end(), *i) != bin_functs.end()) {
			oper = *i;
			i--;
			expr.erase(next(i));
			arg2 = *i;
			i--;
			expr.erase(next(i));
			arg1 = *i;

			if(oper == "*" && is_letter(arg1) && is_letter(arg2)) {
				temp = mat_mult(matrices.at(arg1.c_str()[0]-65), matrices.at(arg2.c_str()[0]-65));
			}
			else if(oper == "+" && is_letter(arg1) && is_letter(arg2)) {
				temp = mat_add(matrices.at(arg1.c_str()[0]-65), matrices.at(arg2.c_str()[0]-65));
			}
			else if(oper == "-" && is_letter(arg1) && is_letter(arg2)) {
				temp = mat_sub(matrices.at(arg1.c_str()[0]-65), matrices.at(arg2.c_str()[0]-65));
			}
			else if(oper == "^" && is_letter(arg1) && arg2 == "t") {
				temp = matrices.at(arg1.c_str()[0]-65);
				temp.transpose();
			}
			else if(oper == "^" && is_letter(arg1) && arg2 == "-1") {
				temp = matrices.at(arg1.c_str()[0]-65);
				temp.inverse();
			}
			else if(oper == "<-" && is_letter(arg1) && is_letter(arg2)) {
				temp = change_of_coords(matrices.at(arg1.c_str()[0]-65), matrices.at(arg2.c_str()[0]-65));
			}
			else if(oper == "." && is_letter(arg1) && is_letter(arg2)) {
				var = 0;
				ostringstream s;
				s << dot_prod(matrices.at(arg1.c_str()[0]-65), matrices.at(arg2.c_str()[0]-65));
				string new_arg = s.str();
				*i = new_arg;
			}
			if(var) {
				string new_arg(1, matrices.size()+65);
				*i = new_arg;
			}
			matrices.push_back(temp);
		}
		else if(find(un_functs.begin(), un_functs.end(), *i) != un_functs.end()) {
			oper = *i;
			i--;
			expr.erase(next(i));
			arg1 = *i;

			if(oper == "u" && is_letter(arg1)) {
				temp = matrices.at(arg1.c_str()[0]-65);
				temp.u();
			}
			else if(oper == "l" && is_letter(arg1)) {
				temp = matrices.at(arg1.c_str()[0]-65);
				temp.l();
			}
			else if(oper == "ref" && is_letter(arg1)) {
				temp = matrices.at(arg1.c_str()[0]-65);
				temp.ref();
			}
			else if(oper == "rref" && is_letter(arg1)) {
				temp = matrices.at(arg1.c_str()[0]-65);
				temp.rref();
			}
			else if(oper == "det" && is_letter(arg1)) {
				var = 0;
				ostringstream s;
				s << matrices.at(arg1.c_str()[0]-65).det();
				string new_arg = s.str();
				*i = new_arg;
			}
			else if(oper == "eval" && is_letter(arg1)) {
				temp = matrices.at(arg1.c_str()[0]-65);
				temp.find_evals(100);
			}
			else if(oper == "gram" && is_letter(arg1)) {
				temp = matrices.at(arg1.c_str()[0]-65);
				temp.gram_schmidt();
			}
			else if(oper == "q" && is_letter(arg1)) {
				temp = matrices.at(arg1.c_str()[0]-65);
				temp.Q();
			}
			else if(oper == "r" && is_letter(arg1)) {
				temp = matrices.at(arg1.c_str()[0]-65);
				temp.R();
			}
			else if(oper == "||" && is_letter(arg1)) {
				var = 0;
				ostringstream s;
				s << matrices.at(arg1.c_str()[0]-65).vec_mag();
				string new_arg = s.str();
				*i = new_arg;
			}
			if(var) {
				string new_arg(1, matrices.size()+65);
				*i = new_arg;
			}
			matrices.push_back(temp);
		}
		i++;
	}
	
	return *expr.begin();
}

void matrix_print(ofstream &o, vector<matrix> matrices, string ans) {
	if(is_num(ans)) {
		o << ans;
	} 
	else if(is_letter(ans)) {
		matrices.at(ans.c_str()[0]-65).print(o);
	}
}

void matrix_solve(ifstream &i, ofstream &o) {
	vector<matrix> matrices;
	string op;
	string ans;
	matrix_read(i, op, matrices);
	ans = matrix_eval(op, matrices);
	matrix_print(o, matrices, ans);
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
