#include "ImageMatchResult.h"

ImageMatchResult::ImageMatchResult(std::string n, int c): name(n), count(c) {

}

int ImageMatchResult::getCount() const {
	return count;
}

std::string ImageMatchResult::getName() const {
	return name;
}

bool ImageMatchResult::operator > (const ImageMatchResult& other) const {
	return count > other.count;
}

