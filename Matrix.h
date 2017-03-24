#ifndef Voting_h
#define Voting_h

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

using namespace std;

void matrix_read(ifstream &ifile, int &op, vector<vector<vector<double>>> &matrices);

int get_op_num(string op_string, string op_arr[]);

void matrix_eval(int &op, vector<vector<vector<double>>> &matrices);

void matrix_print(ofstream &o, vector<vector<vector<double>>> matrices);

void matrix_solve(ifstream &i, ofstream &o);

class matrix {
	public:
		friend matrix mat_mult(matrix m1, matrix m2) {
			vector<vector<double>> m(m1.m, vector<double>(m2.n, 0));
			matrix m3 = matrix(m);

			for(int i = 0; i < m3.m; i++) {
				for(int j = 0; j < m3.n; j++) {
					double a = 0;
					for(int k = 0; k < m1.n; k++) {
						a += m1.mat.at(i).at(k) * m2.mat.at(k).at(j);
					}
					m3.mat.at(i).at(j) = a;
				}
			}
			return m3;
		}
		
	private:
		vector<vector<double>> mat;
		
		int m;
		int n;

		bool swapped;

	public:
		matrix() {
			mat = {};
			m = 0;
			n = 0;
			swapped = false;
		}

		matrix(vector<vector<double>>& _matrix) {
			mat = _matrix;
			m = _matrix.size();
			n = _matrix.at(0).size();
			swapped = false;
		}

		void row_swap(int r1, int r2) {
			vector<double> temp = mat.at(r1);
			mat.at(r1) = mat.at(r2);
			mat.at(r2) = temp;
			swapped = !swapped;
		}

		void row_add(double c, int r1, int r2) {
			for(int i = 0; i < mat.at(r2).size(); i++) {
				mat.at(r2).at(i) += c * mat.at(r1).at(i);
			}
		}

		void row_scale(double c, int r) {
			for(int i = 0; i < mat.at(r).size(); i++) {
				if(mat.at(r).at(i) != 0) {
					mat.at(r).at(i) *= c;
				}
			}
		}

		void ref() {
			int x = 0;
			int y = 0;

			while(x != mat.at(0).size() && y != mat.size()) {
				while(x != mat.at(0).size() && mat.at(y).at(x) == 0) {
					bool swapped = 0;
					for(int i = y+1; i < mat.size(); i++) {
						if(mat.at(i).at(x) != 0) {
							row_swap(i, y);
							swapped = 1;
							break;
						}
					}
					if(swapped == 0) {
						x++;
					}
				}
				if(x >= mat.at(0).size()) {
					break;
				}
				for(int i = 0; i < mat.size(); i++) {
					if(i != y) {
						row_add(-(mat.at(i).at(x)/mat.at(y).at(x)), y, i);
					}
				}
				x++;
				y++;
			}
			
		}

		void rref() {	
			ref();

			for(int i = 0; i < m; i++) {
				for(int j = 0; j < n; j++) {
					if(mat.at(i).at(j) != 0) {
						row_scale(1/mat.at(i).at(j), i);
						break;
					}
				}
			}
		}

		

		void print(ofstream& o) {
			for(int i = 0; i < m; i++) {
				for(int j = 0; j < n; j++) {
					if(j == 0) {
						o << mat.at(i).at(j);
					} else {
						o << " " << mat.at(i).at(j);
					}
				}
				o << "\n";
			}
		}
};

#endif
