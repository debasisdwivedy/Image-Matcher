#include <numeric>
#include <cmath>
#include <algorithm>

#include "utils.h"
using namespace cimg_library;

CImg<double> paste_image(const CImg<double>& input1, const CImg<double>& input2) {
	CImg<double> result(input1.width() + input2.width(), std::max(input1.height(), input2.height()), 1, input1.spectrum(), 0);

	for(int i = 0; i < input1.width(); i++) {
		for(int j = 0; j < input1.height(); j++) {
			for(int k = 0; k < input1.spectrum(); k++) {
				result(i, j, 0, k) = input1(i, j, 0, k);                    
			}
		}
	}

	// second image on the right of the merged image
	for(int i = input1.width(); i < result.width(); i++)
	{
		for(int j = 0; j < input2.height(); j++)
		{
			for(int k = 0; k < input2.spectrum(); k++)
			{
				result(i, j, k) = input2(i - input1.width(), j, k);                    
			}
		}
	}
	return result;
}


