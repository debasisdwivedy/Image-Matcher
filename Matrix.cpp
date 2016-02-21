#include <cstring>
#include <iostream>

#include "Matrix.h"

Matrix::Matrix(int r, int c):rows(r), cols(c) {
	arr = new double*[rows];
	for (int i=0; i<rows; i++) {
		arr[i] = new double[cols];
	}
}

Matrix::Matrix(const Matrix& mat):rows(mat.rows), cols(mat.cols) {
	arr = new double*[rows];
	for (int i=0; i<rows; i++) {
		arr[i] = new double[cols];
		std::memcpy(arr[i], mat.arr[i], cols*sizeof(arr[i][0]));
	}
}

Matrix& Matrix::operator=(const Matrix& mat) {
	if (this == &mat) {
		return *this;
	}
	for (int i=0; i<rows; i++)
		delete[] arr[i];
	delete[] arr;

	arr = new double*[rows];
	for (int i=0; i<rows; i++) {
		arr[i] = new double[cols];
		std::memcpy(arr[i], mat.arr[i], cols*sizeof(arr[i][0]));
	}
	rows = mat.rows;
	cols = mat.cols;
	return *this;
}

const double& Matrix::operator()(int r, int c) const {
	return arr[r][c];
}

double& Matrix::operator()(int r, int c) {
	return arr[r][c];
}

Matrix::~Matrix() {
	for (int i=0; i<rows; i++) {
		delete[] arr[i];
	}
	delete[] arr;
}

void Matrix::print() {
	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			std::cout<<arr[i][j]<<" ";
		}
		std::cout<<std::endl;
	}
}

Matrix& Matrix::operator*=(const Matrix& mat) {
	*this = *this * mat;
	return *this;
}

SqMatrix::SqMatrix(int rows): Matrix(rows, rows), order(rows) {

}

SqMatrix::SqMatrix(const SqMatrix& mat): Matrix(mat), order(mat.order) {
	
}

SqMatrix& SqMatrix::operator=(const SqMatrix& mat) {
	Matrix::operator=(mat);
	order = mat.order;
	return *this;
}

SqMatrix::~SqMatrix() {

}

double SqMatrix::determinant() const {
	/* Reference: https://chi3x10.wordpress.com/2008/05/28/calculate-matrix-inversion-in-c/ */
	if(order == 1)
		return arr[0][0];
	float det = 0;
	
	for (int i=0; i<order; i++) {
		det += (i%2==1? -1.0: 1.0) * arr[0][i] * this->minorMat(0,i).determinant();
	}
	return det;
}

SqMatrix SqMatrix::minorMat(int row, int col) const {
	/* Reference: https://chi3x10.wordpress.com/2008/05/28/calculate-matrix-inversion-in-c/ */
	SqMatrix mat(order-1);
	int curRow = 0, curCol = 0;
	for (int i=0; i<order; i++) {
		if (i == row)
			continue;
		curCol = 0;
		for (int j=0; j<order; j++) {
			if (j == col)
				continue;
			mat.arr[curRow][curCol] = arr[i][j];
			curCol++;
		}
		curRow ++;
	}
	return mat;
}

SqMatrix SqMatrix::inverse() const {
	/* Reference: https://chi3x10.wordpress.com/2008/05/28/calculate-matrix-inversion-in-c/ */
	SqMatrix inv(order);
	double det = determinant();
	for (int i=0; i<order; i++) {
		for (int j=0; j<order; j++) {
			inv.arr[j][i] = minorMat(i,j).determinant() * ((i+j)%2 == 1 ? -1.0 : 1.0) / det;
		}
	}
	return inv;
}

SqMatrix SqMatrix::transpose() const {
	SqMatrix result(order);
	for (int i=0; i<order; i++) {
		for (int j=0; j<order; j++) {
			result(j,i) = arr[i][j];
		}
	}
	return result;
}

SqMatrix& SqMatrix::operator*=(const SqMatrix& mat) {
	Matrix::operator*=(mat);
	return *this;
}

Matrix operator*(const Matrix& m1, const Matrix& m2) {
	if (m1.colCount() != m2.rowCount())
		throw "Dimensions do not match.";
	Matrix result(m1.rowCount(), m2.colCount());
	for (int i=0; i<m1.rowCount(); i++) {
		for (int j=0; j<m2.colCount(); j++) {
			double sum = 0.0;
			for (int k=0; k<m2.rowCount(); k++) {
				sum += m1(i,k) * m2(k,j);
			}
			result(i,j) = sum;
		}
	}
	return result;
}

SqMatrix operator*(const SqMatrix& s1, const SqMatrix& s2) {
	SqMatrix res(s1);
	res *= s2;
	return res;
}


SqMatrix SqMatrix::identity(int order) {
	SqMatrix result(order);
	for (int i=0; i<order; i++) {
		for (int j=0; j<order; j++) {
			result(i,j) = i==j;
		}
	}
	return result;
}
