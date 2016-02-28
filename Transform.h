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
	void rotate(double);

	Transformation inverse() const;
	
	std::pair<double, double> transformCoord(double, double, bool debug=false) const;
};

template <typename T>
void interpolate(const cimg_library::CImg<T>& img, double x, double y, T* result) {
	int x0 = (int)x, y0 = (int)y, x1 = x0 + 1, y1 = y0 + 1;
	std::pair<int, int> points[4] = {
		std::make_pair(x0, y0),
		std::make_pair(x0, y1),
		std::make_pair(x1, y1),
		std::make_pair(x1, y0)
	};
	std::pair<int, int> pixel_locs[4];
	for (int i=0; i<4; i++) {
		int pixelX = points[i].first, pixelY = points[i].second;
		if (pixelX < 0 || pixelX >= img.width() || pixelY < 0 || pixelY >= img.height()) {
			if (pixelY < 0)
				pixelY = -pixelY;
			else if (y >= img.height())
				pixelY = 2 * img.height() - pixelY;

			if (pixelX < 0)
				pixelX = -pixelX;
			else if (x >= img.width())
				pixelX = 2 * img.width() - pixelX;
		}
		pixel_locs[i] = std::make_pair(pixelX, pixelY);
	}
	
	for (int i=0; i<img.spectrum(); i++) {
		result[i] = (x1-x)*(y1-y)*img(pixel_locs[0].first, pixel_locs[0].second,i) +
					(x1-x)*(y-y0)*img(pixel_locs[1].first, pixel_locs[1].second,i) +
					(x-x0)*(y-y0)*img(pixel_locs[2].first, pixel_locs[2].second,i) +
					(x-x0)*(y1-y)*img(pixel_locs[3].first, pixel_locs[3].second,i);
	}
}

template <typename T>
cimg_library::CImg<T> transform_image(const cimg_library::CImg<T>& img, Transformation t) {
	std::pair<double, double> corners[] = {
		std::make_pair(0, 0),
		std::make_pair(0, img.width() - 1),
		std::make_pair(img.height() - 1, 0),
		std::make_pair(img.height() - 1, img.width() - 1)
	};
	std::pair<double, double> updated[] = {
		t.transformCoord(corners[0].first, corners[0].second, true),
		t.transformCoord(corners[1].first, corners[1].second, true),
		t.transformCoord(corners[2].first, corners[2].second, true),
		t.transformCoord(corners[3].first, corners[3].second, true),
	};
	/*double rightMost = std::max(std::max(updated[0].first, updated[1].first),
								std::max(updated[2].first, updated[3].first));
	double leftMost = std::min(std::min(updated[0].first, updated[1].first),
								std::min(updated[2].first, updated[3].first));
	double topMost = std::max(std::max(updated[0].second, updated[1].second),
								std::max(updated[2].second, updated[3].second));
	double bottomMost = std::min(std::min(updated[0].second, updated[1].second),
								std::min(updated[2].second, updated[3].second));

	double widthMapped = rightMost - leftMost, heightMapped = topMost - bottomMost; */
	Transformation inverse = t.inverse();
	//cimg_library::CImg<T> result(widthMapped, heightMapped, 1, img.spectrum(),0);
	cimg_library::CImg<T> result(img.width(), img.height(), 1, img.spectrum(),0);
	
	for (int x=0; x<result.width(); x++) {
		for (int y=0; y<result.height(); y++) {
			std::pair<double, double> coord = inverse.transformCoord(x,y);
			if (coord.first < img.width() && coord.first >= 0 &&
								coord.second >= 0 && coord.second < img.height()) {
				T res[5];
				interpolate(img, coord.first, coord.second, &res[0]);
				for (int c=0; c<img.spectrum(); c++) {
					result(x, y, c) = res[c];
				}
			}
		}
	}
	return result;
}

#endif
