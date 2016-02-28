#include <cmath>
#include <limits>
#include "SiftMatcher.h"
#include "utils.h"

SiftMatcher::SiftMatcher(const Config& c): config(c) {

}

SiftMatcher::~SiftMatcher() {

}

std::vector<std::pair<SiftDescriptor, SiftDescriptor> > SiftMatcher::match(
		const CImg<double>& input1, const CImg<double>& input2) const {
	double sift_distance_threshold = config.get<double>("SiftMatcher.max_dist");
	const unsigned char color[] = {0, 255, 0};

	CImg<double> greyScale1 = input1.spectrum() == 1? input1:input1.get_RGBtoHSI().get_channel(2);
	CImg<double> greyScale2 = input2.spectrum() == 1? input2 : input2.get_RGBtoHSI().get_channel(2);

	std::vector<SiftDescriptor> desc1 = Sift::compute_sift(greyScale1);
	std::vector<SiftDescriptor> desc2 = Sift::compute_sift(greyScale2);

	CImg<double> output;
	if (config.get<int>("SiftMatcher.savejoined") == 1) {
		CImg<double> temp1 = annotate_sift_points(input1, desc1);
		CImg<double> temp2 = annotate_sift_points(input2, desc2);

		output = paste_image(temp1, temp2);
		output.save("sift-joined.png");
	}

	// matching both the descriptors and obtaining the number of matches
	double dist_ratio = config.get<double>("SiftMatcher.dist_ratio");
	std::vector<std::pair<SiftDescriptor, SiftDescriptor> > result;
	for(size_t i = 0; i < desc1.size(); i++) {
		double closestIndex=0, secondClosestIndex=0;
		double closestDist = std::numeric_limits<double>::infinity(),
			   secondClosestDist = std::numeric_limits<double>::infinity();
		for(size_t j = 0; j < desc2.size(); j++) {
			double distance = descriptor_distance(desc1[i], desc2[j]);

			if(distance < closestDist) {
				secondClosestDist = closestDist;
				secondClosestIndex = closestIndex;
				closestDist = distance;
				closestIndex = j;
			}
		}
		if (closestDist/secondClosestDist < dist_ratio) {
			output.draw_line(desc1[i].col, desc1[i].row, 
				desc2[closestIndex].col + input1.width(), desc2[closestIndex].row, color);

			result.push_back(std::make_pair(desc1[i], desc2[closestIndex]));
		}
	}

	// saving the sift output to a image file
	if(config.get<int>("SiftMatcher.savelines") == 1) {
		output.save("sift.png");
	}

	return result;
}


double descriptor_distance(const SiftDescriptor& s1, const SiftDescriptor& s2) {
	double sum = 0;
	for(int k = 0; k < 128; k++) {
		double delta = s1.descriptor[k] - s2.descriptor[k];
		sum += delta * delta;
	}
	return std::sqrt(sum);
}


cimg_library::CImg<double> annotate_sift_points(const CImg<double>& image, const std::vector<SiftDescriptor>& descriptors) {
	CImg<double> result(image);
	for(size_t i=0; i<descriptors.size(); i++) {
		/*std::cout<<"descriptor #"<<i<<": x="<<descriptors[i].col 
			<<" y="<<descriptors[i].row<<" descriptor=(";
		for(int l=0; l<128; l++)
			std::cout << descriptors[i].descriptor[l] << "," ;
		std::cout<<")"<<std::endl;
		*/
		for(size_t j=0; j<5; j++) {
			for(size_t k=0; k<5; k++) {
				if(j==2 || k==2) {
					for(size_t p=0; p<result.spectrum(); p++) {
						if(descriptors[i].col+k < result.width() && 
									descriptors[i].row+j < result.height()) {
							result(descriptors[i].col+k, descriptors[i].row+j, 0, p)=0;
						}
					}
				}
			}
		}
	}
	return result;
}

MappedCoordinates getMappedCoordinate(const std::pair<SiftDescriptor, SiftDescriptor>& p) {
	MappedCoordinates mc = {
		p.first.col, p.first.row, 
		p.second.col, p.second.row
	};
	return mc;
}

std::ostream& operator<<(std::ostream& out, const SiftDescriptor& desc) {
	out<<"Descriptor: x=" << desc.col << " y=" << desc.row << " descriptor=(";
	for(int l=0; l<128; l++)
		out<<desc.descriptor[l]<<",";
	out<<")"<<std::endl;
	return out;
}

