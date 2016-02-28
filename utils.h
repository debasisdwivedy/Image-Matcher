#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdlib>

#include "CImg.h"
#include "Sift.h"

cimg_library::CImg<double> paste_image(const cimg_library::CImg<double>&, const cimg_library::CImg<double>&);


/* Reference: http://stackoverflow.com/a/6943003/227884 */
template <typename I>
I random_element(I begin, I end)
{
    const unsigned long n = std::distance(begin, end);
    const unsigned long divisor = (RAND_MAX + 1) / n;

    unsigned long k;
    do { k = std::rand() / divisor; } while (k >= n);

    std::advance(begin, k);
    return begin;
}

#endif
