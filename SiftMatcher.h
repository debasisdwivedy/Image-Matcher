#ifndef __SIFT_MATCHER_H
#define __SIFT_MATCHER_H

#include <vector>

#include "Config.h"
#include "MappedCoordinates.h"
#include "CImg.h"
#include "Sift.h"

class SiftMatcher {
	const Config& config;

public:
	SiftMatcher(const Config&);
	virtual ~SiftMatcher();
	
	std::vector<std::pair<SiftDescriptor, SiftDescriptor> > match(
			const cimg_library::CImg<double>&, const CImg<double>&) const;
};

double descriptor_distance(const SiftDescriptor&, const SiftDescriptor&);

MappedCoordinates getMappedCoordinate(const std::pair<SiftDescriptor, SiftDescriptor>&);

cimg_library::CImg<double> annotate_sift_points(
		const cimg_library::CImg<double>&, const std::vector<SiftDescriptor>&);
std::ostream& operator<<(std::ostream& out, const SiftDescriptor& desc);

#endif
