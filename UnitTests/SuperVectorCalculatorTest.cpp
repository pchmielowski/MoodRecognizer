#pragma warning(push, 0)
#include <boost/test/unit_test.hpp>
#include <fakeit.hpp>
#include <string>
#include "opencv/cv.h"
#include "UbmLoader.h"
#pragma warning(pop)
#include "FeatureMatrixLoader.h"
#include "SuperVectorCalculator.h"
#include "UbmLoader.h"
using namespace fakeit;
using namespace std;

class SuperVectorCalculatorTestFixture
{
public:
	Mock<FeatureMatrixLoader> featureMatrixLoader;
	FeatureMatrixLoader& featureMatrixLoaderFactory(FeatureMatrix& simpleFeatureMatrix) {
		When(Method(featureMatrixLoader, get)).AlwaysReturn(simpleFeatureMatrix);
		return featureMatrixLoader.get();
	}
private:

};

BOOST_FIXTURE_TEST_SUITE(SuperVectorCalculatorTest, SuperVectorCalculatorTestFixture)
BOOST_AUTO_TEST_CASE(calculate_1gaussComponentSimpleMatrix)
{
	FeatureMatrix simpleFeatureMatrix = (cv::Mat_<float>(1, 1) << 1);
	Mock<FeatureMatrixLoader> featureMatrixLoader;
	When(Method(featureMatrixLoader, get)).AlwaysReturn(simpleFeatureMatrix);
	FeatureMatrixLoader& featureMatrixLoaderInstance = featureMatrixLoader.get();

	int numDimensions = simpleFeatureMatrix.rows;
	Ubm ubm;
	ubm.numGaussComponents_ = 1;
	ubm.means_ = (cv::Mat_<float>(numDimensions, ubm.numGaussComponents_) << 2);
	ubm.weights_ = (cv::Mat_<float>(numDimensions, ubm.numGaussComponents_) << 1);
	vector<Mat> covs;
	covs.push_back(cv::Mat_<float>(numDimensions, numDimensions) << 3);
	ubm.createNormalDistribution(numDimensions, covs);

	Mock<UbmLoader> ubmLoader;
	When(Method(ubmLoader, getUbm)).AlwaysReturn(ubm);
	UbmLoader& ubmLoaderInstance = ubmLoader.get();

	vector<Alpha> alphas;
	alphas.push_back(1.0);

	SuperVectorCalculator SUT(featureMatrixLoaderInstance, ubmLoaderInstance, alphas);
	SuperVectors result = SUT.calculate("any_name");
}
BOOST_AUTO_TEST_CASE(calculate_1gaussComponentBiggerMatrix)
{
	FeatureMatrix simpleFeatureMatrix = (cv::Mat_<float>(4, 1) << 1, 2, 3, 4);
	Mock<FeatureMatrixLoader> featureMatrixLoader;
	When(Method(featureMatrixLoader, get)).AlwaysReturn(simpleFeatureMatrix);
	FeatureMatrixLoader& featureMatrixLoaderInstance = featureMatrixLoader.get();

	int numDimensions = simpleFeatureMatrix.rows;
	Ubm ubm;
	ubm.numGaussComponents_ = 1;
	ubm.means_ = (cv::Mat_<float>(numDimensions, ubm.numGaussComponents_) << 1, 4, 0, 4);
	ubm.weights_ = (cv::Mat_<float>(1, ubm.numGaussComponents_) << 1);
	vector<Mat> covs;
	covs.push_back((cv::Mat_<float>(numDimensions, 1) << 1, 1, 1, 1));
	ubm.createNormalDistribution(numDimensions, covs);

	Mock<UbmLoader> ubmLoader;
	When(Method(ubmLoader, getUbm)).AlwaysReturn(ubm);
	UbmLoader& ubmLoaderInstance = ubmLoader.get();

	vector<Alpha> alphas;
	alphas.push_back(1.0);

	SuperVectorCalculator SUT(featureMatrixLoaderInstance, ubmLoaderInstance, alphas);
	SuperVectors result = SUT.calculate("any_name");
}
BOOST_AUTO_TEST_CASE(calculate_1gaussComponent2dAlpha0)
{
	FeatureMatrix simpleFeatureMatrix = (cv::Mat_<float>(2, 1) << 2, 4);
	Mock<FeatureMatrixLoader> featureMatrixLoader;
	When(Method(featureMatrixLoader, get)).AlwaysReturn(simpleFeatureMatrix);
	FeatureMatrixLoader& featureMatrixLoaderInstance = featureMatrixLoader.get();

	int numDimensions = simpleFeatureMatrix.rows;
	Ubm ubm;
	ubm.numGaussComponents_ = 1;
	ubm.means_ = (cv::Mat_<float>(numDimensions, ubm.numGaussComponents_) << -1, 4);
	ubm.weights_ = (cv::Mat_<float>(1, ubm.numGaussComponents_) << 1);
	vector<Mat> covs;
	covs.push_back((cv::Mat_<float>(numDimensions, 1) << 13, .25));
	ubm.createNormalDistribution(numDimensions, covs);

	Mock<UbmLoader> ubmLoader;
	When(Method(ubmLoader, getUbm)).AlwaysReturn(ubm);
	UbmLoader& ubmLoaderInstance = ubmLoader.get();

	vector<Alpha> alphas;
	alphas.push_back(0.0);

	SuperVectorCalculator SUT(featureMatrixLoaderInstance, ubmLoaderInstance, alphas);
	SuperVectors result = SUT.calculate("any_name");

	BOOST_CHECK_CLOSE(result[0].at<float>(0), -1, 0.1);
	BOOST_CHECK_CLOSE(result[0].at<float>(1), 4, 0.1);
}
BOOST_AUTO_TEST_CASE(calculate_1gaussComponent2dAlpha1)
{
	FeatureMatrix simpleFeatureMatrix = (cv::Mat_<float>(2, 1) << 2, 4);
	FeatureMatrixLoader* featureMatrixLoaderInstance = &featureMatrixLoaderFactory(simpleFeatureMatrix);

	int numDimensions = simpleFeatureMatrix.rows;
	Ubm ubm;
	ubm.numGaussComponents_ = 1;
	ubm.means_ = (cv::Mat_<float>(numDimensions, ubm.numGaussComponents_) << -1, 4);
	ubm.weights_ = (cv::Mat_<float>(1, ubm.numGaussComponents_) << 1);
	vector<Mat> covs;
	covs.push_back((cv::Mat_<float>(numDimensions, 1) << 13, .25));
	ubm.createNormalDistribution(numDimensions, covs);

	Mock<UbmLoader> ubmLoader;
	When(Method(ubmLoader, getUbm)).AlwaysReturn(ubm);
	UbmLoader& ubmLoaderInstance = ubmLoader.get();

	vector<Alpha> alphas;
	alphas.push_back(1.0);

	SuperVectorCalculator SUT(*featureMatrixLoaderInstance, ubmLoaderInstance, alphas);
	SuperVectors result = SUT.calculate("any_name");

	BOOST_CHECK_CLOSE(result[0].at<float>(0), 2, 0.1);
	BOOST_CHECK_CLOSE(result[0].at<float>(1), 4, 0.1);
}
BOOST_AUTO_TEST_CASE(calculate_1gaussComponent2dAlpha1_2timeWindows)
{
	FeatureMatrix simpleFeatureMatrix = (cv::Mat_<float>(2, 2) << 1, 2, 3, 4);
	FeatureMatrixLoader* featureMatrixLoaderInstance = &featureMatrixLoaderFactory(simpleFeatureMatrix);

	int numDimensions = simpleFeatureMatrix.rows;
	Ubm ubm;
	ubm.numGaussComponents_ = 1;
	ubm.means_ = (cv::Mat_<float>(numDimensions, ubm.numGaussComponents_) << -1, 4);
	ubm.weights_ = (cv::Mat_<float>(1, ubm.numGaussComponents_) << 1);
	vector<Mat> covs;
	covs.push_back((cv::Mat_<float>(numDimensions, 1) << 13, .25));
	ubm.createNormalDistribution(numDimensions, covs);

	Mock<UbmLoader> ubmLoader;
	When(Method(ubmLoader, getUbm)).AlwaysReturn(ubm);
	UbmLoader& ubmLoaderInstance = ubmLoader.get();

	vector<Alpha> alphas;
	alphas.push_back(1.0);

	SuperVectorCalculator SUT(*featureMatrixLoaderInstance, ubmLoaderInstance, alphas);
	SuperVectors result = SUT.calculate("any_name");

	//BOOST_CHECK_CLOSE(result[0].at<float>(0), 1.5, 0.1);
	//BOOST_CHECK_CLOSE(result[0].at<float>(1), 3.5, 0.1);
}
BOOST_AUTO_TEST_CASE(calculate_2gaussComponents1dAlpha1)
{
	FeatureMatrix simpleFeatureMatrix = (cv::Mat_<float>(1, 1) << 2);
	FeatureMatrixLoader* featureMatrixLoaderInstance = &featureMatrixLoaderFactory(simpleFeatureMatrix);

	int numDimensions = simpleFeatureMatrix.rows;
	Ubm ubm;
	ubm.numGaussComponents_ = 2;
	ubm.means_ = (cv::Mat_<float>(numDimensions, ubm.numGaussComponents_) << -1, 4);
	ubm.weights_ = (cv::Mat_<float>(1, ubm.numGaussComponents_) << .9, .1);
	vector<Mat> covs;
	cv::Mat myDiagonal = (cv::Mat_<float>(numDimensions, 1) << 3);
	covs.push_back(cv::Mat::diag(myDiagonal));
	covs.push_back(cv::Mat::diag(myDiagonal));
	ubm.createNormalDistribution(numDimensions, covs);

	Mock<UbmLoader> ubmLoader;
	When(Method(ubmLoader, getUbm)).AlwaysReturn(ubm);
	UbmLoader& ubmLoaderInstance = ubmLoader.get();

	vector<Alpha> alphas;
	alphas.push_back(1.0);

	SuperVectorCalculator SUT(*featureMatrixLoaderInstance, ubmLoaderInstance, alphas);
	SuperVectors result = SUT.calculate("any_name");
}
BOOST_AUTO_TEST_CASE(calculate_1gaussComponent2dAlpha1and0)
{
	FeatureMatrix simpleFeatureMatrix = (cv::Mat_<float>(2, 1) << 2, 4);
	FeatureMatrixLoader* featureMatrixLoaderInstance = &featureMatrixLoaderFactory(simpleFeatureMatrix);

	int numDimensions = simpleFeatureMatrix.rows;
	Ubm ubm;
	ubm.numGaussComponents_ = 1;
	ubm.means_ = (cv::Mat_<float>(numDimensions, ubm.numGaussComponents_) << -1, 4);
	ubm.weights_ = (cv::Mat_<float>(1, ubm.numGaussComponents_) << 1);
	vector<Mat> covs;
	covs.push_back((cv::Mat_<float>(numDimensions, 1) << 13, .25));
	ubm.createNormalDistribution(numDimensions, covs);

	Mock<UbmLoader> ubmLoader;
	When(Method(ubmLoader, getUbm)).AlwaysReturn(ubm);
	UbmLoader& ubmLoaderInstance = ubmLoader.get();

	vector<Alpha> alphas;
	alphas.push_back(1.0);
	alphas.push_back(0.0);

	SuperVectorCalculator SUT(*featureMatrixLoaderInstance, ubmLoaderInstance, alphas);
	SuperVectors result = SUT.calculate("any_name");

	BOOST_CHECK_CLOSE(result[0].at<float>(0), 2, 0.1);
	BOOST_CHECK_CLOSE(result[0].at<float>(1), 4, 0.1);
	BOOST_CHECK_CLOSE(result[1].at<float>(0), -1, 0.1);
	BOOST_CHECK_CLOSE(result[1].at<float>(1), 4, 0.1);
}

BOOST_AUTO_TEST_SUITE_END()