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
	std::vector<std::vector<boost::math::normal_distribution<>>> distribution_;
	double logLikelihood(const cv::Mat& x, int gaussComponentIdx) const {
		assert(x.cols == 1);
		assert(x.rows > 0);
		int numCoeff = x.rows;
		assert(distribution_.size() == numGaussComponents_);
		assert(distribution_[0].size() == numCoeff);
		assert(distribution_[numGaussComponents_-1].size() == numCoeff);
		double likelihood = 1.0;
		for (int coefIdx = 0; coefIdx < numCoeff; coefIdx++) {
			likelihood *= pdf(distribution_[gaussComponentIdx][coefIdx],
				x.at<double>(coefIdx));
		}
		return log(likelihood);
	}
	void createNormalDistribution(int numDimensions, vector<Mat> covs)
	{
		assert(means_.rows == numDimensions);
		assert(means_.cols == numGaussComponents_);
		assert(covs.size() == numGaussComponents_);
		assert(covs[0].rows == numDimensions);
		assert(covs[0].cols == numDimensions);
		if (numDimensions > 1) {
			bool seemsToBeDiagonal = (abs(covs[0].at<double>(0, 1)) <= 1e-5) 
				&& (abs(covs[0].at<double>(1, 0)) <= 1e-5);
			assert(seemsToBeDiagonal);
		}
		distribution_.resize(numGaussComponents_, 
			vector<normal_distribution<>>(numDimensions, NULL));
		for (int dimensionIdx = 0; dimensionIdx < numDimensions; dimensionIdx++) {
			for (int componentIdx = 0; componentIdx < numGaussComponents_; componentIdx++) {
				distribution_[componentIdx][dimensionIdx] = normal_distribution<>(means_.at<double>(dimensionIdx, componentIdx),
					(covs[componentIdx].at<double>(dimensionIdx, dimensionIdx)));
			}
		}
	}
};