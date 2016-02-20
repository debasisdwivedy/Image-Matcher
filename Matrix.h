#ifndef __MAdoubleRIX_H__
#define __MAdoubleRIX_H__

#include <iostream>

class Matrix {
protected:
	double** arr;
	int rows, cols;
public:
	Matrix(int rows, int cols);
	Matrix(const Matrix&);
	Matrix& operator=(const Matrix&);
	virtual ~Matrix();

	const double& operator()(int r, int c) const;
	double& operator()(int r, int c);

	Matrix& operator*=(const Matrix&);

	int rowCount() const { return rows; }
	int colCount() const { return cols; }


	void print();
};

class SqMatrix: public Matrix {
protected:
	int order;
public:
	SqMatrix(int rows);
	SqMatrix(const SqMatrix&);
	SqMatrix& operator=(const SqMatrix&);
	virtual ~SqMatrix();

	double determinant();
	SqMatrix minor(int row, int col);
	SqMatrix inverse();

	SqMatrix& operator*=(const SqMatrix&);
};

Matrix operator*(const Matrix&, const Matrix&);
SqMatrix operator*(const SqMatrix&, const SqMatrix&);

#endif

