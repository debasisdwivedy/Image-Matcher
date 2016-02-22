#include <iostream>
#include <cmath>

#include "CImg.h"
#include "Transform.h"

Transformation::Transformation(const SqMatrix& mat): curMatrix(mat) {
	if (curMatrix.rowCount() != 3)
		throw "Unable to handle matrices of dimension != 3.";
}

void Transformation::translate(double x, double y) {
	curMatrix(0,2) += x;
	curMatrix(1,2) += y;
}

void Transformation::rotate(double theta) {
	SqMatrix mat = SqMatrix::identity(3);
	mat(0,0) = std::cos(theta);
	mat(0,1) = -std::sin(theta);
	mat(1,0) = std::sin(theta);
	mat(1,1) = std::cos(theta);
	curMatrix *= mat;
}

std::pair<double, double> Transformation::transformCoord(double x, double y, bool debug) const {
	Matrix m2(3, 1);
	m2(0,0) = x;
	m2(1,0) = y;
	m2(2,0) = 1;
	Matrix result(curMatrix * m2);
	result(0,0) /= result(2,0);
	result(1,0) /= result(2,0);
	result(2,0) /= result(2,0);

	if (debug) {
		std::cout<<"==========\n";
		std::cout<<"Input: ("<<x<<", "<<y<<")\n";
		std::cout<<"CurMatrix:\n";
		curMatrix.print();
		std::cout<<"------\n";
		std::cout<<"Multiplier\n";
		m2.print();
		std::cout<<"------\n";
		std::cout<<"Result:\n";
		result.print();
		std::cout<<"======\n\n";
	}
	return std::make_pair(result(0,0), result(1,0));
}


Transformation Transformation::inverse() const {
	return curMatrix.inverse();
}
