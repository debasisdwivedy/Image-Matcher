#ifndef __SIFT_MATCHER_H
#define __SIFT_MATCHER_H

#include <vector>

#include "Config.h"
#include "MappedCoordinates.h"
#include "CImg.h"
#include "Sift.h"

class SiftMatcher {
	const Config& config;

protected:
	static cimg_library::CImg<double> annotate_sift_points(
			const cimg_library::CImg<double>&, const std::vector<SiftDescriptor>&);

	static double descriptor_distance(const SiftDescriptor&, const SiftDescriptor&);
public:
	SiftMatcher(const Config&);
	virtual ~SiftMatcher();
	
	std::vector<MappedCoordinates> match(
			const cimg_library::CImg<double>&, const CImg<double>&) const;

};


std::ostream& operator<<(std::ostream& out, const SiftDescriptor& desc);

#endif
