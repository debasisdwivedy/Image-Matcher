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

double descriptor_distance(const SiftDescriptor& s1, const SiftDescriptor& s2) {
	double sum = 0;
	for(int k = 0; k < 128; k++) {
		sum += std::pow((s1.descriptor[k]-s2.descriptor[k]),2);
	}
	return std::sqrt(sum);
}


cimg_library::CImg<double> annotate_sift_points(const CImg<double>& image, const std::vector<SiftDescriptor>& descriptors) {
	CImg<double> result(image);
	for(int i=0; i<descriptors.size(); i++) {
		for(int j=0; j<5; j++) {
			for(int k=0; k<5; k++) {
				if(j==2 || k==2) {
					for(int p=0; p<3; p++) {
						if(descriptors[i].col+k < result.width() && descriptors[i].row+j < result.height()) {
							result(descriptors[i].col+k, descriptors[i].row+j, 0, p)=0;
						}
					}
				}
			}
		}
	}
	return result;
}

std::ostream& operator<<(std::ostream& out, const SiftDescriptor& desc) {
	out<<"Descriptor: x=" << desc.col << " y=" << desc.row << " descriptor=(";
	for(int l=0; l<128; l++)
		out<<desc.descriptor[l]<<",";
	out<<")"<<std::endl;
	return out;
}

