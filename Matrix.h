#include <list>
#include <limits>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <functional>

using namespace std;

void matrix_read(ifstream &ifile, int &op, vector<vector<vector<double>>> &matrices);

int get_op_num(string op_string, string op_arr[]);

void matrix_eval(int &op, vector<vector<vector<double>>> &matrices);

void matrix_print(ofstream &o, vector<vector<vector<double>>> matrices, string ans);

void matrix_solve(ifstream &i, ofstream &o);


class matrix {
	public:
		//returns product of two matrices
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

		//returns dot product of two vectors
		friend double dot_prod(matrix m1, matrix m2) {
			if(m1.m != m2.m) {
				throw invalid_argument("Dot product: m1 columns do not match m2 columns.");
			}
			if(m1.n != 1 || m2.n != 1) {
				throw invalid_argument("Dot product: m1 or m2 is not a vector");
			}
			matrix temp = m1;
			temp.transpose();
			return mat_mult(temp, m2).mat.at(0).at(0);
		}

		//returns sum of two matrices
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

		//returns difference of two matrices
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

		//returns change of coordinates matrix from m1 to m2; P(m2 <- m1)
		friend matrix change_of_coords(matrix m2, matrix m1) {
			if(m1.m != m2.m || m1.n != m2.n) {
				throw invalid_argument("Change of coordinates: m1 dimensions do not match m2 dimensions");
			}
			matrix m3(m1.m, m1.n+m2.n);
			for(int i = 0; i < m1.m; i++) {
				for(int j = 0; j < m1.n+m2.n; j++) {
					m3.mat.at(i).at(j) = (j < m2.n ? m2.mat.at(i).at(j) : m1.mat.at(i).at(j - m1.n));
				}
			}
			m3.rref();

			matrix m4(m1.n, m1.n);
			for(int i = 0; i < m1.n; i++) {
				for(int j = 0; j < m1.n; j++) {
					m4.mat.at(i).at(j) = m3.mat.at(i).at(j + m1.n);
				}
			}
			return m4;
		}

		
	private:
		vector<vector<double>> mat;
		
		int m;
		int n;

		bool swapped;

		vector<vector<double>> l_mat;

	public:
		//default constructor
		matrix() {
			mat = {};
			l_mat = {};
			m = 0;
			n = 0;
			swapped = false;
		}

		//accepts 2d vector as arg and creates matrix obj
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

		//accepts dimensions as args and creates matrix obj with those dimensions
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

		//creates square identity matrix with given dimension
		matrix(int s) {
			mat = vector<vector<double>> (s, vector<double> (s, 0));
			m = s;
			n = s;
			for(int i = 0; i < m; i++) {
				mat.at(i).at(i) = 1;
			}
			l_mat = mat;
		}

		//creates a matrix by appending a bunch of vectors (of the matrix variety) together
		matrix(vector<matrix> &vecs) {
			mat = vector<vector<double>> (vecs.at(0).mat.size(), vector<double> (vecs.size(), 0));
			m = mat.size();
			n = mat.at(0).size();
			for(int i = 0; i < m; i++) {
				for(int j = 0; j < n; j++) {
					mat.at(i).at(j) = vecs.at(j).mat.at(i).at(0);
				}
			}
			if(m == n) {
				l_mat = matrix(m).mat;
			} else {
				l_mat = {};
			}
		}

		//swaps rows r1 and r2
		void row_swap(int r1, int r2) {
			vector<double> temp = mat.at(r1);
			mat.at(r1) = mat.at(r2);
			mat.at(r2) = temp;
			swapped = !swapped;
		}

		//equivalent to r2 = r2 + c * r1
		void row_add(double c, int r1, int r2) {
			for(int i = 0; i < mat.at(r2).size(); i++) {
				mat.at(r2).at(i) += c * mat.at(r1).at(i);
			}
		}

		//equivalent to r = c * r
		void row_scale(double c, int r) {
			for(int i = 0; i < mat.at(r).size(); i++) {
				if(mat.at(r).at(i) != 0) {
					mat.at(r).at(i) *= c;
				}
			}
		}

		//scales matrix by some scalar c
		void mat_scale(double c) {
			for(int i = 0; i < m; i++) {
				for(int j = 0; j < n; j++) {
					mat.at(i).at(j) *= c;
				}
			}
		}

		//converts *this to a matrix where row i and column j are removed
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

		//converts *this to upper triangular matrix
		void u() {
			if(m != n) {
				throw invalid_argument("m1 is not a square matrix.");
			}

			int x = 0;
			int y = 0;

			while(x != mat.at(0).size() && y != mat.size()) {
				//if [x, y] equals 0 and there is a non-0 value under [x, y] then swap the two rows
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
				//zero out all rows in current column under [x, y] and build lower tri matrix
				for(int i = y+1; i < mat.size(); i++) {
					l_mat.at(i).at(x) = mat.at(i).at(x)/mat.at(y).at(x);
					row_add(-(mat.at(i).at(x)/mat.at(y).at(x)), y, i);
				}
				x++;
				y++;
			}
			
		}

		//converts *this to lower triangular matrix
		void l() {
			u();
			mat = l_mat;
		}

		//converts *this to matrix where every row in a column above and below the first occurence of a non-zero number in a row is zeroed out
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

		//converts *this to row reduced echelon form
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
	

		//returns determinant of *this
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

		//returns determinant of *this with row i and column j removed
		double det(int i, int j) {
			matrix temp = *this;
			temp.mat_ij(i, j);
			return temp.det();
		}

		//converts *this to inverse of *this using cramer's rule
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

		//converts *this to matrix transpose of *this	
		void transpose() {
			matrix temp (n, m);
			
			for(int i = 0; i < m; i++) {
				for(int j = 0; j < n; j++) {
					temp.mat.at(j).at(i) = mat.at(i).at(j);
				}
			}
			
			*this = temp;
		}

		//returns vector number vecnum from matrix (index starting at 0)
		matrix get_vec(int vecnum) {
			matrix vec(this->m, 1);
			for(int i = 0; i < this->m; i++) {
				vec.mat.at(i).at(0) = mat.at(i).at(vecnum);
			}
			return vec;
		}

		//returns magnitude of vector
		double vec_mag() {
			if(n != 1) {
				throw invalid_argument("Vector Magnitude: not a vector");
			}
			return sqrt(dot_prod(*this, *this));
		}

		//converts *this into basis for space that spans *this
		void basis() {
			matrix temp = *this;
			temp.rref();
			vector<matrix> vecs = {};

			for(int i = 0; i < m; i++) {
				for(int j = 0; j < n; j++) {
					if(temp.mat.at(i).at(j) != 0) {
						vecs.push_back(get_vec(j));
						break;
					}
				}
			}

			*this = matrix(vecs);
		}		

		//converts *this into basis for null space of *this
		void null_space() {
			matrix temp = *this;
			temp.rref();
			vector<matrix> vecs = {};

			for(int i = 0; i < m; i++) {
				for(int j = 0; j < n; j++) {
					if(temp.mat.at(i).at(j) != 0) {
						temp.row_swap(i, j);
						break;
					}
				}
			}
			if(temp.m > temp.n) {
				for(int i = m; i > n; i--) {
					temp.mat.pop_back();
					temp.m--;
				}
			}
			else if(temp.n > temp.m) {
				vector<double> zeroes (temp.n, 0);
				for(int i = m; i < n; i++) {
					temp.mat.push_back(zeroes);
					temp.m++;
				}
			}
			for(int i = 0; i < temp.m; i++) {
				if(temp.mat.at(i).at(i) == 0) {
					matrix vec = temp.get_vec(i);
					vec.mat_scale(-1);
					vec.mat.at(i).at(0) = 1;
					vecs.push_back(vec);
				}
			}

			*this = matrix(vecs);
		}

		//converts *this to matrix with orthogonal columns using gram-schmidt process
		void gram_schmidt() {
			matrix x;

			for(int i = 0; i < this->n; i++) {
				x = get_vec(i);
				for(int j = 0; j < i; j++) {
					matrix v = get_vec(j);
					v.mat_scale(dot_prod(x, v) / dot_prod(v, v));
					x = mat_sub(x, v);
				}
				for(int j = 0; j < this->m; j++) {
					mat.at(j).at(i) = x.mat.at(j).at(0);
				}
			}
		}

		//converts *this to orthogonal matrix (orthonormal columns) such that A = QR
		void Q() {
			gram_schmidt();
			double scale;

			for(int i = 0; i < this->n; i++) {
				scale = get_vec(i).vec_mag();
				for(int j = 0; j < this->m; j++) {
					mat.at(j).at(i) /= scale;
				}
			}
		}

		//converts *this to upper triangular matrix such that A = QR
		void R() {
			matrix q = *this;
			q.Q();
			q.transpose();
			*this = mat_mult(q, *this);
		}	

		//returns vector containing eigenvalues using the QR process
		matrix find_evals(int iters) {
			matrix q, r, a;
			a = *this;
			for(int i = 0; i < iters; i++) {
				q = r = a;
				q.Q();
				r.R();
				a = mat_mult(r, q);
			}
			matrix ret(a.m, 1);
			for(int i = 0; i < a.m; i++) {
				ret.mat.at(i).at(0) = a.mat.at(i).at(i);
			}
			return ret;
		}


		//rounds every value in *this to p decimal places
		void mat_round(int p) {
			for(int i = 0; i < m; i++) {
				for(int j = 0; j < n; j++) {
					mat.at(i).at(j) = round(mat.at(i).at(j) * pow(10, p)) / pow(10, p);
					if(mat.at(i).at(j) == 0) {
						mat.at(i).at(j) = 0;
					}
				}
			}
		}

		//outputs *this into o and rounds all numbers to d decimal places
		void print(ostream& o, int d) {
			mat_round(d);

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

		//outputs *this into o and rounds all numbers to 5 decimal places
		void print(ostream& o) {
			print(o, 5);
		}
};

