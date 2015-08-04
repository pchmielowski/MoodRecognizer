#pragma once;
#include <vector>
#include <boost/math/distributions/normal.hpp>
#include "opencv/cv.h"

class Ubm
{
public:
	int numGaussComponents;
	cv::Mat weights;
	cv::Mat means;
	std::vector<std::vector<boost::math::normal_distribution<>>> norm_distr;
	double logLikelihood(const cv::Mat& x, int gaussComponentIdx) const {
		double likelihood = 1;
		int numCoeff = x.rows;
		for (int coefIdx = 0; coefIdx < numCoeff; coefIdx++) {
			// czy to nie powinien byæ log likelihood?
			likelihood *= pdf(norm_distr[gaussComponentIdx][coefIdx], 
				x.at<double>(coefIdx));
		}
		return log(likelihood);
	}
};