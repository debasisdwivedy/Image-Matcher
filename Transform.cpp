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

std::pair<double, double> Transformation::transformCoord(double x, double y) const {
	Matrix m2(3, 1);
	m2(0,0) = x;
	m2(1,0) = y;
	m2(2,0) = 1;

	Matrix result(curMatrix * m2);
	return std::make_pair(result(0,0), result(1,0));
}


Transformation Transformation::inverse() const {
	return curMatrix.inverse();
}
