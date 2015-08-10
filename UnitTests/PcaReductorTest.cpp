#pragma warning(push, 0)
#include "boost/test/unit_test.hpp"
#include <fakeit.hpp>
#pragma warning(pop)
// My classes:
#include "Types.h"
#include "PcaReductor.h"
#include "opencv/cv.h"
#include <vector>
#include <iostream>
#define _CRT_SECURE_NO_WARNINGS

using namespace cv;

class PcaReductorTestFixture {
private:
	PcaReductor* pcaReductor_ = nullptr;
public:
	PcaReductor& pcaReductorFactoryWithTwo2dVectors(Mat v1, Mat v2) {
		std::vector<int> nOfComponents = { 2 };
		pcaReductor_ =  new PcaReductor(nOfComponents);

		SuperVectors superVectors;
		superVectors.push_back(v1);
		superVectors.push_back(v2);
		pcaReductor_->trainPca(superVectors);

		return *pcaReductor_;
	}
	~PcaReductorTestFixture() {
		delete pcaReductor_;
	}
};

BOOST_FIXTURE_TEST_SUITE(PcaReductorTest, PcaReductorTestFixture)
BOOST_AUTO_TEST_CASE(reduce_two2dVectorsTo1dVector)
{
	PcaReductor& pcaReductor = pcaReductorFactoryWithTwo2dVectors(
		(Mat_<float>(2, 1) << 1, 0),
		(Mat_<float>(2, 1) << -1, 0));

	SuperVector reducedSuperVector = pcaReductor.reduce((cv::Mat_<float>(2, 1) << -1, 0));

	BOOST_CHECK_CLOSE(reducedSuperVector.at<float>(0), -1, .01);
	BOOST_CHECK_EQUAL(reducedSuperVector.cols, 1);
	//BOOST_CHECK_EQUAL(reducedSuperVector.rows, 1);
}
BOOST_AUTO_TEST_CASE(reduce_two2dVectorsTo1dVector2)
{
	PcaReductor& pcaReductor = pcaReductorFactoryWithTwo2dVectors(
		(Mat_<float>(2, 1) << 1, 0),
		(Mat_<float>(2, 1) << -1, 0));

	SuperVector reducedSuperVector = pcaReductor.reduce((cv::Mat_<float>(2, 1) << 1, 0));

	BOOST_CHECK_CLOSE(reducedSuperVector.at<float>(0), 1, .01);
}
BOOST_AUTO_TEST_CASE(reduce_two2dVectorsTo1dVector3)
{
	PcaReductor& pcaReductor = pcaReductorFactoryWithTwo2dVectors(
		(Mat_<float>(2, 1) << 0, 1),
		(Mat_<float>(2, 1) << 0, -1));

	SuperVector reducedSuperVector1 = pcaReductor.reduce((cv::Mat_<float>(2, 1) << 5, 0));
	SuperVector reducedSuperVector2 = pcaReductor.reduce((cv::Mat_<float>(2, 1) << -5, 0));
	SuperVector reducedSuperVector3 = pcaReductor.reduce((cv::Mat_<float>(2, 1) << 0, 5));
	SuperVector reducedSuperVector4 = pcaReductor.reduce((cv::Mat_<float>(2, 1) << 0, -5));


	//BOOST_CHECK_CLOSE(reducedSuperVector.at<float>(0), -0.97014, .01);
}
BOOST_AUTO_TEST_SUITE_END()