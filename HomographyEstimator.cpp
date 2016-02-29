#include "HomographyEstimator.h"
#include "utils.h"
#include "Transform.h"

namespace {
	SqMatrix cimg_to_sqmat(const CImg<double> input) {
		if (input.width() != input.height()) {
			throw "Expected a square CImg";
		}
		SqMatrix result(input.height());
		for (int i=0; i<input.height(); i++) {
			for (int j=0; j<input.width(); j++) {
				result(i,j) = input(j,i);
			}
		}
		return result;
	}
#if 0
	void print(CImg<double> mat) {
		for (int i=0; i<mat.height(); i++) {
			for (int j=0; j<mat.width(); j++) {
				std::cout<<mat(j,i)<<" ";
			}
			std::cout<<std::endl;
		}
	}
#endif

	std::pair<CImg<double>, CImg<double> > get_matrix(MappedCoordinates input[4]) {
#if 0
		std::cout<<input[0].x1<<", "<<input[0].y1<<" => "<<input[0].x2<<", "<<input[0].y2<<std::endl;
		std::cout<<input[1].x1<<", "<<input[1].y1<<" => "<<input[1].x2<<", "<<input[1].y2<<std::endl;
		std::cout<<input[2].x1<<", "<<input[2].y1<<" => "<<input[2].x2<<", "<<input[2].y2<<std::endl;
		std::cout<<input[3].x1<<", "<<input[3].y1<<" => "<<input[3].x2<<", "<<input[3].y2<<std::endl;
#endif
		//Return matrices A and B from the equation: AX = B;
		CImg<double> matA(8, 8), matB(1,8);
		for (int i=0; i<4; i++) {
			matA(0, i*2) = input[i].x1;
			matA(1, i*2) = input[i].y1;
			matA(2, i*2) = 1;
			matA(3, i*2) = 0;
			matA(4, i*2) = 0;
			matA(5, i*2) = 0;
			matA(6, i*2) = -(input[i].x1 * input[i].x2);
			matA(7, i*2) = -(input[i].y1 * input[i].x2);

			matA(0, i*2+1) = 0;
			matA(1, i*2+1) = 0;
			matA(2, i*2+1) = 0;
			matA(3, i*2+1) = input[i].x1;
			matA(4, i*2+1) = input[i].y1;
			matA(5, i*2+1) = 1;
			matA(6, i*2+1) = -(input[i].x1 * input[i].y2);
			matA(7, i*2+1) = -(input[i].y1 * input[i].y2);

			matB(0, i*2) = input[i].x2;
			matB(0, i*2+1) = input[i].y2;
		}
#if 0
		std::cout<<"A Matrix: "<<std::endl;
		print(matA);
		std::cout<<"B Matrix: "<<std::endl;
		print(matB);
#endif
		return std::make_pair(matA, matB);
	}

	SqMatrix solve_maps(MappedCoordinates input[4]) {
		std::pair<CImg<double>, CImg<double> > mats = get_matrix(input);
#if 0
		std::cout<<"Solving A:"<<std::endl;
		print(mats.first);
		std::cout<<"Solving B:"<<std::endl;
		print(mats.second);
#endif
		
		CImg<double> result = mats.second.get_solve(mats.first);
		SqMatrix res(3);
		res(0,0) = result(0,0);
		res(0,1) = result(0,1);
		res(0,2) = result(0,2);
		res(1,0) = result(0,3);
		res(1,1) = result(0,4);
		res(1,2) = result(0,5);
		res(2,0) = result(0,6);
		res(2,1) = result(0,7);
		res(2,2) = 1.0;
#if 0
		std::cout<<"Result: "<<std::endl;
		print(result);
#endif
		return res;
	}

	void choose_4_points(const std::vector<MappedCoordinates>& input, 
			MappedCoordinates* result, const Config& config) {
		
		while (true) {
			std::vector<MappedCoordinates> other_maps(input);
			MappedCoordinates current_map[4];
			for (int i=0; i<4; i++) {
					std::vector<MappedCoordinates>::iterator it = random_element(other_maps.begin(), other_maps.end());
					//std::cout<<"Removing index: "<<(it - other_maps.begin())<<std::endl;
					current_map[i] = *it;
					other_maps.erase(it);
			}
			CImg<double> matA = get_matrix(current_map).first;
			SqMatrix mat = cimg_to_sqmat(matA);
			if (mat.determinant() * mat.inverse().determinant() < 10) {
				std::cout<<"return good set of 4 points."<<std::endl;
				for (int i=0; i<4; i++)
					result[i] = current_map[i];
				return;
			}
		}
	}

	std::pair<int, SqMatrix> test_hypothesis(const std::vector<MappedCoordinates>& input, const Config& config) {
		std::vector<MappedCoordinates> other_maps(input);
		MappedCoordinates selected_maps[4];
#if 0
		for (int i=0; i<4; i++) {
			std::vector<MappedCoordinates>::iterator it = random_element(other_maps.begin(), other_maps.end());
			//std::cout<<"Removing index: "<<(it - other_maps.begin())<<std::endl;
			selected_maps[i] = *it;
			other_maps.erase(it);
		}
#endif
		std::cout<<"Choosing 4 points..";
		choose_4_points(input, selected_maps, config);

		//Solve AX = B
		SqMatrix result = solve_maps(selected_maps);
		Transformation transform(result);
		int inlier_count=0, outlier_count=0;
		double max_dist = config.get<double>("ransac.max_dist");
		for (std::vector<MappedCoordinates>::iterator it = other_maps.begin(); it != other_maps.end(); it++) {
			std::pair<double, double> res = transform.transformCoord(it->x1, it->y1);
			double dist = std::sqrt(std::pow(res.first - it->x2, 2) + std::pow(res.second - it->y2,2));
			if (dist <= max_dist) {
				inlier_count ++;
			} else {
				outlier_count++;
			}
		}
		return std::make_pair(inlier_count, result);
	}
}

SqMatrix estimate_homography(const std::vector<MappedCoordinates>& input, const Config& config) {
	double ransac_reqd_prob = config.get<double>("ransac.reqd_prob");
	double ransac_prob_inliner = config.get<double>("ransac.inlier_prob");
	// ln(1 - p)/ln(1-(1-e)^s) >= N
	int trial_count = 1 + std::log(1-ransac_reqd_prob) / std::log(1-std::pow(1-ransac_prob_inliner, 4));
	std::cout<<"Running "<<trial_count<<" trials."<<std::endl;
	std::pair<int, SqMatrix> best(-1, SqMatrix(3));
	for (int i=0; i<trial_count; i++) {
		std::cout<<"Trial #"<<i<<": ";
		std::pair<int, SqMatrix> res = test_hypothesis(input, config);
		std::cout<<"Number of inliers: "<<res.first<<
					" out of "<<input.size()-4<<std::endl;
		if (res.first > best.first) {
			std::cout<<"Selecting Matrix:"<<std::endl;
			best = res;
			best.second.print();
			std::cout<<std::endl;
		}
	}

	return best.second;
}

