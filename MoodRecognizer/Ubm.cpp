#include "Ubm.h"



float Ubm::logLikelihood(const cv::Mat& x, int gaussComponentIdx) const
{
	assert(x.cols == 1);
	assert(x.rows > 0);
	int numCoeff = x.rows;
	assert(distribution_.size() == numGaussComponents_);
	assert(distribution_[0].size() == numCoeff);
	assert(distribution_[numGaussComponents_ - 1].size() == numCoeff);
	double likelihood = 1.0;
	for (int coefIdx = 0; coefIdx < numCoeff; coefIdx++) {
		likelihood *= pdf(distribution_[gaussComponentIdx][coefIdx],
			x.at<float>(coefIdx));
	}
	return static_cast<float>(log(likelihood));
}

void Ubm::createNormalDistribution(int numDimensions, vector<Mat> covs)
{
	assert(means_.rows == numDimensions);
	assert(means_.cols == numGaussComponents_);
	assert(covs.size() == numGaussComponents_);
	assert(covs[0].rows == numDimensions);
	assert(covs[0].cols == 1);
	//if (numDimensions > 1) {
	//	bool seemsToBeDiagonal = (abs(covs[0].at<float>(0, 1)) <= 1e-5)
	//		&& (abs(covs[0].at<float>(1, 0)) <= 1e-5);
	//	assert(seemsToBeDiagonal);
	//}
	distribution_.resize(numGaussComponents_,
		vector<normal_distribution<>>(numDimensions, NULL));
	for (int dimensionIdx = 0; dimensionIdx < numDimensions; dimensionIdx++) {
		for (int componentIdx = 0; componentIdx < numGaussComponents_; componentIdx++) {
			distribution_[componentIdx][dimensionIdx] = 
				normal_distribution<>(means_.at<float>(dimensionIdx, componentIdx),
				(covs[componentIdx].at<float>(dimensionIdx, 0)));
		}
	}
}
