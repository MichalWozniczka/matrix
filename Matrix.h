
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <sstream>
#include <stdexcept>

using namespace std;

void matrix_read(ifstream &ifile, int &op, vector<vector<vector<double>>> &matrices);

int get_op_num(string op_string, string op_arr[]);

void matrix_eval(int &op, vector<vector<vector<double>>> &matrices);

void matrix_print(ofstream &o, vector<vector<vector<double>>> matrices, string ans);

void matrix_solve(ifstream &i, ofstream &o);

class matrix {
	public:
		friend matrix mat_mult(matrix m1, matrix m2) {
			if(m1.n != m2.m) {
				throw invalid_argument("Matrix Mult: m1 columns do not match m2 rows.");
			}

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

		friend matrix mat_add(matrix m1, matrix m2) {
			if(m1.m != m2.m || m1.n != m2.n) {
				throw invalid_argument("Matrix add: m1 dimensions do not match m2 dimensions.");
			}

			vector<vector<double>> m(m1.m, vector<double>(m1.n, 0));
			matrix m3 = matrix(m);

			for(int i = 0; i < m3.m; i++) {
				for(int j = 0; j < m3.n; j++) {
					m3.mat.at(i).at(j) = m1.mat.at(i).at(j) + m2.mat.at(i).at(j);
				}
			}
			return m3;
		}

		friend matrix mat_sub(matrix m1, matrix m2) {
			if(m1.m != m2.m || m1.n != m2.n) {
				throw invalid_argument("Matrix sub: m1 dimensions do not match m2 dimensions.");
			}		

			vector<vector<double>> m(m1.m, vector<double>(m1.n, 0));
			matrix m3 = matrix(m);

			for(int i = 0; i < m3.m; i++) {
				for(int j = 0; j < m3.n; j++) {
					m3.mat.at(i).at(j) = m1.mat.at(i).at(j) - m2.mat.at(i).at(j);
				}
			}
			return m3;
		}
		
	private:
		vector<vector<double>> mat;
		
		int m;
		int n;

		bool swapped;

		vector<vector<double>> l_mat;

	public:
		matrix() {
			mat = {};
			l_mat = {};
			m = 0;
			n = 0;
			swapped = false;
		}

		matrix(vector<vector<double>>& _matrix) {
			mat = _matrix;
			m = _matrix.size();
			n = _matrix.at(0).size();
			
			if(m == n) {
				l_mat = vector<vector<double>> (m, vector<double> (n, 0));
				for(int i = 0; i < m; i++) {
					l_mat.at(i).at(i) = 1;
				}
			} else {
				l_mat = {};
			}

			swapped = false;
		}

		matrix(int _m, int _n) {
			mat = vector<vector<double>> (_m, vector<double> (_n, 0));
			m = _m;
			n = _n;

			if(m == n) {
				l_mat = vector<vector<double>> (m, vector<double> (n, 0));
				for(int i = 0; i < m; i++) {
					l_mat.at(i).at(i) = 1;
				}
			} else {
				l_mat = {};
			}

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

		void mat_scale(double c) {
			for(int i = 0; i < m; i++) {
				for(int j = 0; j < n; j++) {
					mat.at(i).at(j) *= c;
				}
			}
		}

		void mat_ij(int i, int j) {
			matrix temp (m-1, n-1);

			int _x = 0;
			int _y = 0;
			for(int y = 0; y < m; y++) {
				_x = 0;
				if(i != y) {
					for(int x = 0; x < n; x++) {
						if(j != x) {
							temp.mat.at(_y).at(_x) = mat.at(y).at(x);
							_x++;
						}
					}
					_y++;
				}
			}
			*this = temp;
		}					

		void u() {
			if(m != n) {
				throw invalid_argument("m1 is not a square matrix.");
			}

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
				for(int i = y+1; i < mat.size(); i++) {
					l_mat.at(i).at(x) = mat.at(i).at(x)/mat.at(y).at(x);
					row_add(-(mat.at(i).at(x)/mat.at(y).at(x)), y, i);
				}
				x++;
				y++;
			}
			
		}

		void l() {
			u();
			mat = l_mat;
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

		double det() {
			matrix temp = *this;
			temp.ref();

			double det = 1;
			for(int i = 0; i < m; i++) {
				det *= temp.mat.at(i).at(i);
			}
			if(temp.swapped) {
				det = -det;
			}
			return det;
		}

		double det(int i, int j) {
			matrix temp = *this;
			temp.mat_ij(i, j);
			return temp.det();
		}

		void inverse() {
			int sign;
			matrix temp (m, n);
			for(int i = 0; i < m; i++) {
				for(int j = 0; j < n; j++) {
					(i+j)%2 == 0 ? sign = 1 : sign = -1;
					temp.mat.at(j).at(i) = sign * det(i, j);
				}
			}
			temp.mat_scale(1/det());
			*this = temp;
		}
					
	
		void transpose() {
			matrix temp (n, m);
			
			for(int i = 0; i < m; i++) {
				for(int j = 0; j < n; j++) {
					temp.mat.at(j).at(i) = mat.at(i).at(j);
				}
			}
			
			*this = temp;
		}

		void round(int p) {
			for(int i = 0; i < m; i++) {
				for(int j = 0; j < n; j++) {
					stringstream ss;
					ss << fixed;
					ss.precision(p);
					ss << mat.at(i).at(j);
					ss >> mat.at(i).at(j);
					if(mat.at(i).at(j) == 0) {
						mat.at(i).at(j) = 0;
					}
				}
			}
		}

		void print(ofstream& o) {
			round(3);

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

