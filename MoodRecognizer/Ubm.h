#pragma once;
#include <vector>
#include <boost/math/distributions/normal.hpp>
#include "opencv/cv.h"

using namespace boost::math;
using namespace cv;
using namespace std;
class Ubm
{
public:
	int numGaussComponents_;
	cv::Mat weights_;
	cv::Mat means_;
	vector<Mat> covs_;
	std::vector<std::vector<boost::math::normal_distribution<>>> distribution_;
	double weightedLogLikelihood(const cv::Mat& x, int gaussComponentIdx) const;
	void createNormalDistribution(int numDimensions, vector<Mat> covs);
};