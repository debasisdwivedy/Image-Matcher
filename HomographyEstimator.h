#ifndef __HOMOGRAPHY_H__
#define __HOMOGRAPHY_H__

#include "Matrix.h"

SqMatrix estimate_homography(const std::vector<MappedCoordinates>&);

#endif
