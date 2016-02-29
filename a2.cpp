// B657 assignment 2 skeleton code
//
// Compile with: "make"
//
// See assignment handout for command line and project specifications.


//Link to the header file
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <list>
#include <functional>
#include <iomanip>
#include <algorithm>

#include "CImg.h"
#include "Sift.h"
#include "Matrix.h"
#include "Transform.h"
#include "utils.h"
#include "FastSiftMatcher.h"
#include "MappedCoordinates.h"
#include "ImageMatchResult.h"
#include "HomographyEstimator.h"


//Use the cimg namespace to access the functions easily
using namespace cimg_library;
using namespace std;

void img_display(const CImg<double>& img) {
	cimg_library::CImgDisplay main_disp(img, "temp");
	while (!main_disp.is_closed() )
		main_disp.wait();

}

int main(int argc, char **argv)
{
	std::srand(time(0));
	//return temp();
	Config config("config.txt");
	try {
		if(argc < 2) {
			cout << "Insufficent number of arguments; correct usage:" << endl;
			cout << "    a2-p1 part_id ..." << endl;
			return -1;
		}

		string part = argv[1];
		
		// Sift detector
		if (part == "sift") {
			if (argc < 4) {
				std::cout<<"Insufficent number of arguments."<<std::endl;
				return -1;
			}
			config.set("SiftMatcher.savelines", "1");
			CImg<double> img1(argv[2]), img2(argv[3]);
			SiftMatcher matcher(config);
			std::vector<std::pair<SiftDescriptor, SiftDescriptor> > res = matcher.match(img1, img2);
			std::cout<<" ["<<res.size()<<" matches]"<<std::endl;
			img_display(CImg<double>("sift.png"));
		} else if(!part.compare(0, 5,"part1")) {
			if(argc < 4) {
				cout << "Insufficent number of arguments." << endl;
				return -1;
			}

			CImg<double> query(argv[2]);
			// creating a vector to store the sift matches for each of the image
			std::vector<ImageMatchResult> matchResult;

			// take in all the images from the cammand line and calculate sift matches of the descriptors
			SiftMatcher full_matcher(config);
			FastSiftMatcher fast_matcher(config);

			SiftMatcher* matcher = part == "part1Fast"? &fast_matcher: &full_matcher;
			for(int i=3; i<argc; i++)
			{
				std::cout<<"Processing: "<<argv[i];
				CImg<double> img(argv[i]);
				std::vector<std::pair<SiftDescriptor, SiftDescriptor> > result(matcher->match(query, img));
				matchResult.push_back(ImageMatchResult(argv[i], result.size()));
				std::cout<<" ["<<result.size()<<" matches]"<<std::endl;
			} 

			// sort according to the sift match count
			std::sort(matchResult.begin(), matchResult.end(), std::greater<ImageMatchResult>());

			// printing the results
			cout<<"Top 10 matches for Input Image: "<<argv[2]<<endl<<std::setw(50)<<"Image Name:"<<setw(10)<<"Count: "<<endl;
			for(size_t i = 0; i < matchResult.size() && i < 10; i++) {
				std::cout<<std::setw(50)<<matchResult[i].getName()<<std::setw(10)<<matchResult[i].getCount()<<std::endl;
			}
		}
		else if(part == "part2") {
			if(argc < 4) {
				cout << "Insufficent number of arguments." << endl;
				return -1;
			}

			CImg<double> query(argv[2]);
			SiftMatcher matcher(config);
			for(int i=3; i<argc; i++) {
				std::cout<<"Processing: "<<argv[i]<<std::endl;
				CImg<double> img(argv[i]);
				std::vector<std::pair<SiftDescriptor, SiftDescriptor> > result(matcher.match(img, query));
				std::vector<MappedCoordinates> mapped;
				for (size_t j=0; j<result.size(); j++) {
					mapped.push_back(getMappedCoordinate(result[j]));
				}
				SqMatrix mat(estimate_homography(mapped, config));
				std::cout<<"Transformation Matrix:\n"<<std::endl;
				mat.print();
				Transformation t(mat);
				CImg<double> transformed_image(transform_image(img, t));
				transformed_image.save((std::string(argv[i]) + "-warped.png").c_str());
			}
		}
		else
			throw std::string("unknown part!");

	}
	catch(const string &err) {
		cerr << "Error: " << err << endl;
	}
}


