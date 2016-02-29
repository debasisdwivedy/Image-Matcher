#include "FastSiftMatcher.h"

namespace {
	template <typename P, typename Q>
	double vector_dot(const std::vector<P>& a, const std::vector<Q>& b) {
		double sum = 0;
		if (a.size() != b.size()) {
			return false;
		}

		for (size_t i=0; i<a.size(); i++) {
			sum += a[i] * b[i];
		}
		return sum;
	}
	std::vector<double> project_sift(const SiftDescriptor& s, 
			const std::vector<std::vector<double> >& x) {
		std::vector<double> result;
		for (size_t i=0; i<x.size(); i++) {
			double dot_product = vector_dot(s.descriptor, x[i]);
			result.push_back(dot_product);
		}
		return result;
	}

	double vector_dist(const std::vector<double>& a, const std::vector<double>& b) {
		double sum = 0;
		if (a.size() != b.size()) {
			return -1;
		}

		for (size_t i=0; i<a.size(); i++) {
			sum += std::pow(a[i] - b[i], 2);
		}
		return std::sqrt(sum);
	}
}

FastSiftMatcher::FastSiftMatcher(const Config& c): SiftMatcher(c) {
	param_k = config.get<int>("FastSiftMatcher.k");
	param_w = config.get<double>("FastSiftMatcher.w");
	param_max_dist = config.get<double>("FastSiftMatcher.max_dist");
	CImg<double> rand_mat(param_k, 128);
	rand_mat.rand(0,1.0);
	for (int i=0; i<param_k; i++) {
		std::vector<double> rand_vector;
		for (int j=0; j<128; j++) {
			rand_vector.push_back(rand_mat(j, i));
		}
		x_vector.push_back(rand_vector);
	}
}

FastSiftMatcher::~FastSiftMatcher() {
}


std::vector<SiftDescriptor> FastSiftMatcher::get_comparables(const SiftDescriptor& desc1,
			const std::vector<SiftDescriptor>& desc2) const {
	std::vector<SiftDescriptor> result;

	std::vector<double> desc1_signature = project_sift(desc1, x_vector);

	for (std::vector<SiftDescriptor>::const_iterator it=desc2.begin(); it != desc2.end(); it ++) {
		std::vector<double> desc_signature = project_sift(*it, x_vector);
		if (vector_dist(desc_signature, desc1_signature) < param_max_dist) {
			result.push_back(*it);
		}
	}

	return result;
}


