#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdlib>
#include <iostream>

#include "CImg.h"
#include "Sift.h"

cimg_library::CImg<double> paste_image(const cimg_library::CImg<double>&, const cimg_library::CImg<double>&);


/* Reference: http://stackoverflow.com/a/6943003/227884 */
template <typename I>
I random_element(I begin, I end)
{
#if 0
    const unsigned long n = std::distance(begin, end);
    const unsigned long divisor = (RAND_MAX + 1) / n;

    unsigned long k;
    do { k = std::rand() / divisor; } while (k >= n);
	std::cout<<"Random element index: "<<k<<std::endl;
    std::advance(begin, k);
    return begin;
#endif
	size_t len = std::distance(begin, end);
	int index = std::rand() % len;

	std::advance(begin, index);
	return begin;
}

template <typename T>
void print(CImg<T> img) {
	for (int i=0; i<img.height(); i++) {
		for (int j=0; j<img.width(); j++) {
			std::cout<<img(j, i)<<" ";
		}
		std::cout<<std::endl;
	}
}

#endif
