#ifndef __FAST_SIFT_MATCHER_H
#define __FAST_SIFT_MATCHER_H

#include "SiftMatcher.h"


class FastSiftMatcher: public SiftMatcher {
	double param_w, param_k, param_max_dist;
	std::vector<std::vector<double> > x_vector;

protected:
	std::vector<SiftDescriptor> get_comparables(const SiftDescriptor&,
				const std::vector<SiftDescriptor>&) const;

public:
	FastSiftMatcher(const Config& config);
	virtual ~FastSiftMatcher();
};

#endif
