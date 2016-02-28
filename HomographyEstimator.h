#ifndef __HOMOGRAPHY_H__
#define __HOMOGRAPHY_H__

#include <vector>
#include "Matrix.h"
#include "MappedCoordinates.h"
#include "Config.h"

SqMatrix estimate_homography(const std::vector<MappedCoordinates>&, const Config&);

#endif
