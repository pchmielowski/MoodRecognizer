#pragma warning(push, 0)
#include <boost/test/unit_test.hpp>
#include <fakeit.hpp>

#include "opencv/cv.h"
#include "UbmLoader.h"
#include <boost/math/distributions/normal.hpp>
#include <vector>
#pragma warning(pop)
using namespace std;
using namespace boost::math;
using namespace cv;

vector<vector<normal_distribution<>>> createNormalDistribution(int numDimensions, int numGaussComponents, Mat means, vector<Mat> covs)
{
	vector<vector<normal_distribution<>>> norm_distr;
	norm_distr.resize(numGaussComponents, vector<normal_distribution<>>(numDimensions, NULL));
	for (int dimensionIdx = 0; dimensionIdx < numDimensions; dimensionIdx++) {
		for (int componentIdx = 0; componentIdx < numGaussComponents; componentIdx++) {
			norm_distr[componentIdx][dimensionIdx] = normal_distribution<>(means.at<double>(dimensionIdx, componentIdx),
				sqrt(covs[componentIdx].at<double>(dimensionIdx, dimensionIdx)));
		}
	}
	return norm_distr;
}

BOOST_AUTO_TEST_SUITE(UbmTest)
BOOST_AUTO_TEST_CASE(likelihood_1gaussComponent1d) {
	Ubm ubm;
	ubm.means = (cv::Mat_<double>(1, 1) << 0);
	ubm.weights = (cv::Mat_<double>(1, 1) << 1);
	ubm.numGaussComponents = 1;
	int numDimensions = 1;
	vector<Mat> covs;
	covs.push_back((cv::Mat_<double>(1, 1) << 1));
	ubm.norm_distr = createNormalDistribution(
		numDimensions, ubm.numGaussComponents, ubm.means, covs);
	Mat x = (cv::Mat_<double>(1, 1) << 0);

	double result = ubm.logLikelihood(x, 0);

	BOOST_CHECK_CLOSE(result, -0.91894, .1);
}
BOOST_AUTO_TEST_CASE(likelihood_2gaussComponents1d) {
	Ubm ubm;
	ubm.means = (cv::Mat_<double>(1, 2) << 0, 1);
	ubm.weights = (cv::Mat_<double>(1, 2) << .5, .5);
	ubm.numGaussComponents = 2;
	int numDimensions = 1;
	vector<Mat> covs;
	covs.push_back(cv::Mat_<double>(1, 1) << 1);
	covs.push_back(cv::Mat_<double>(1, 1) << 1);
	ubm.norm_distr = createNormalDistribution(
		numDimensions, ubm.numGaussComponents, ubm.means, covs);
	Mat x = (cv::Mat_<double>(1, 1) << 0);

	double result = ubm.logLikelihood(x, 0);

	//BOOST_CHECK_CLOSE(result, 0.4, 1);
}
BOOST_AUTO_TEST_SUITE_END()
