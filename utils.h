#ifndef __UTILS_H__
#define __UTILS_H__

#include "CImg.h"
#include "Sift.h"

cimg_library::CImg<double> paste_image(const cimg_library::CImg<double>&, const cimg_library::CImg<double>&);

cimg_library::CImg<double> annotate_sift_points(const CImg<double>& image, const std::vector<SiftDescriptor>&);

double descriptor_distance(const SiftDescriptor&, const SiftDescriptor&);

#endif
