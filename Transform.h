#ifndef __IMAGE_HOMOGRAPHY_H
#define __IMAGE_HOMOGRAPHY_H

#include <utility>
#include <iostream>

#include "Matrix.h"
#include "CImg.h"

class Transformation {
	SqMatrix curMatrix;

public:
	Transformation(const SqMatrix&);

	void translate(double, double);

	Transformation inverse() const;
	
	std::pair<double, double> transformCoord(double, double) const;
};

template <typename T>
cimg_library::CImg<T> transform_image(const cimg_library::CImg<T>& img, Transformation t) {
	std::pair<double, double> corners[] = {
		std::make_pair(0, 0),
		std::make_pair(0, img.width() - 1),
		std::make_pair(img.height() - 1, 0),
		std::make_pair(img.height() - 1, img.width() - 1)
	};
	std::pair<double, double> mapped[] = {
		t.transformCoord(corners[0].first, corners[0].second),
		t.transformCoord(corners[1].first, corners[1].second),
		t.transformCoord(corners[2].first, corners[2].second),
		t.transformCoord(corners[3].first, corners[3].second),
	};
	double leftMost = std::min(std::min(mapped[0].first, mapped[1].first), 
								std::min(mapped[2].first, mapped[3].first));
	double bottomMost = std::min(std::min(mapped[0].second, mapped[1].second),
								std::min(mapped[2].second, mapped[3].second));
	t.translate(leftMost < 0 ? -leftMost: 0, bottomMost < 0 ? -bottomMost: 0);

	std::pair<double, double> updated[] = {
		t.transformCoord(corners[0].first, corners[0].second),
		t.transformCoord(corners[1].first, corners[1].second),
		t.transformCoord(corners[2].first, corners[2].second),
		t.transformCoord(corners[3].first, corners[3].second),
	};
	for (int i=0; i<4; i++) {
		std::cout<<"Corner #"<<i<<": "
			"("<<corners[i].first<<","<<corners[i].second<<") =>"
			"("<<mapped[i].first<<","<<mapped[i].second<<") =>"
			"("<<updated[i].first<<","<<updated[i].second<<")"<<std::endl;
	}
	double rightMost = std::max(std::max(updated[0].first, updated[1].first),
								std::max(updated[2].first, updated[3].first));
	double topMost = std::max(std::max(updated[0].second, updated[1].second),
								std::max(updated[2].second, updated[3].second));

	Transformation inverse = t.inverse();
	cimg_library::CImg<double> result(rightMost,topMost,1,3,0);
#if 1
	cimg_forXYC(img, x, y, c) {
		std::pair<double, double> coord = t.transformCoord(x,y);
		//std::cout<<"Map: "<<x<<", "<<y<<" => "<<coord.first<<", "<<coord.second<<std::endl;
		int row = (int)coord.first, col = (int)coord.second;
		if (row < result.width() && row >= 0 && col >= 0 && col < result.height())
			result(row, col, c) = img(x, y, c);
	}
#else
	cimg_forXYC(result, x, y, c) {
		std::pair<double, double> coord = inverse.transformCoord(x,y);
		//std::cout<<"Map: "<<x<<", "<<y<<" => "<<coord.first<<", "<<coord.second<<std::endl;
		int row = (int)coord.first, col = (int)coord.second;
		if (row < img.width() && row >= 0 && col >= 0 && col < img.height())
			result(x,y,c) = img(row, col, c);
	}
#endif
	return result;
}

#endif
