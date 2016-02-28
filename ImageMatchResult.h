#ifndef __IMAGE_MATCH_RESULT_H
#define __IMAGE_MATCH_RESULT_H

#include <string>
// holds the name of the image file and the matching score of sift descriptors
class ImageMatchResult {
	std::string name;
	int count;
public:
	ImageMatchResult(std::string name, int count); 
	int getCount() const;
	std::string getName() const;
	bool operator > (const ImageMatchResult& i1) const;
};

#endif
