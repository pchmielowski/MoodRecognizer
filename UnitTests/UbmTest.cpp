#pragma warning(push, 0)
#include <boost/test/unit_test.hpp>
//#include <fakeit.hpp>
#include "opencv/cv.h"
#include <boost/math/distributions/normal.hpp>
#include <vector>
#pragma warning(pop)

#include "UbmLoader.h"

using namespace std;
using namespace cv;

BOOST_AUTO_TEST_SUITE(UbmTest)
BOOST_AUTO_TEST_CASE(likelihood_1gaussComponent1d) {
	Ubm ubm;
	ubm.means_ = (cv::Mat_<float>(1, 1) << 2);
	ubm.numGaussComponents_ = 1;
	int numDimensions = 1;
	vector<Mat> covs;
	covs.push_back((cv::Mat_<float>(1, 1) << 3));
	ubm.createNormalDistribution(
		numDimensions, covs);
	Mat x = (cv::Mat_<float>(1, 1) << 5);

	float result = ubm.logLikelihood(x, 0);

	BOOST_CHECK_CLOSE(result, -2.5176, .01);
}
BOOST_AUTO_TEST_CASE(likelihood_2gaussComponents1d) {
	Ubm ubm;
	ubm.means_ = (cv::Mat_<float>(1, 2) << 0, 1);
	ubm.numGaussComponents_ = 2;
	int numDimensions = 1;
	vector<Mat> covs;
	covs.push_back(cv::Mat_<float>(1, 1) << 1);
	covs.push_back(cv::Mat_<float>(1, 1) << 1);
	ubm.createNormalDistribution(
		numDimensions, covs);
	Mat x = (cv::Mat_<float>(1, 1) << 0);

	float result0 = ubm.logLikelihood(x, 0);
	float result1 = ubm.logLikelihood(x, 1);

	BOOST_CHECK_CLOSE(result0, -0.918938533204673, .01);
	BOOST_CHECK_CLOSE(result1, -1.4189, .01);
}
BOOST_AUTO_TEST_CASE(likelihood_1gaussComponent2d) {
	Ubm ubm;
	ubm.means_ = (cv::Mat_<float>(2, 1) << 7.5, .3);
	ubm.numGaussComponents_ = 1;
	int numDimensions = 2;
	vector<Mat> covs;
	covs.push_back((cv::Mat_<float>(2, 1) << 2.0, 100.2));
	ubm.createNormalDistribution(
		numDimensions, covs);
	Mat x = (cv::Mat_<float>(2, 1) << 2, -23);

	float result = ubm.logLikelihood(x, 0);

	BOOST_CHECK_CLOSE(result, -10.946, .01);
}
BOOST_AUTO_TEST_CASE(likelihood_2gaussComponents2d) {
	Ubm ubm;
	ubm.means_ = (cv::Mat_<float>(2, 2) << 0, 0, -4, 4);
	ubm.numGaussComponents_ = 2;
	int numDimensions = 2;
	vector<Mat> covs;
	covs.push_back((cv::Mat_<float>(2, 1) << 4, 4));
	covs.push_back((cv::Mat_<float>(2, 1) << 285.0, 48));
	ubm.createNormalDistribution(
		numDimensions, covs);
	Mat x = (cv::Mat_<float>(2, 1) << 66, -66);

	float result0 = ubm.logLikelihood(x, 0);
	float result1 = ubm.logLikelihood(x, 1);

	BOOST_CHECK_CLOSE(result0, -276.860465788649, 10);
	BOOST_CHECK_CLOSE(result1, -12.4550984402575, .1);
}
BOOST_AUTO_TEST_SUITE_END()
