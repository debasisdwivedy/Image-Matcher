#include "HomographyEstimator.h"
#include "utils.h"
#include "Transform.h"

namespace {
	std::pair<CImg<double>, CImg<double> > get_matrix(MappedCoordinates input[4]) {
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
			matA(3, i*2) = input[i].x1;
			matA(4, i*2) = input[i].y1;
			matA(5, i*2) = 1;
			matA(6, i*2) = -(input[i].x1 * input[i].y2);
			matA(7, i*2) = -(input[i].y1 * input[i].y2);

			matB(0, i*2) = input[i].x2;
			matB(0, i*2+1) = input[i].y2;
		}
		return std::make_pair(matA, matB);
	}

	SqMatrix solve_maps(MappedCoordinates input[4]) {
		std::pair<CImg<double>, CImg<double> > mats = get_matrix(input);
		CImg<double> result = mats.second.solve(mats.first);
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

		return res;
	}

	std::pair<int, SqMatrix> test_hypothesis(const std::vector<MappedCoordinates>& input, const Config& config) {
		std::vector<MappedCoordinates> other_maps(input);
		MappedCoordinates selected_maps[4];
		for (int i=0; i<4; i++) {
			std::vector<MappedCoordinates>::iterator it = random_element(other_maps.begin(), other_maps.end());
			selected_maps[i] = *it;
			other_maps.erase(it);
		}

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
	int trial_count = 50;
	std::pair<int, SqMatrix> best(-1, SqMatrix(3));
	for (int i=0; i<trial_count; i++) {
		std::pair<int, SqMatrix> res = test_hypothesis(input, config);
		if (res.first > best.first) {
			std::cout<<"Selecting Matrix:"<<std::endl;
			best = res;
			best.second.print();
			std::cout<<std::endl;
		}
	}

	return best.second;
}

int main() {
	MappedCoordinates mp[4] = {
		{0, 0, -182, 58},
		{0, 1023, 46.878, 876.02},
		{680, 0, 548.995, -58.1372},
		{680, 1023, 453.776, 926.836}
	};
	solve_maps(mp).print();
}
