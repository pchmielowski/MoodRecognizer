#pragma warning(push, 0)
#include "boost/test/unit_test.hpp"
#include <fakeit.hpp>
#pragma warning(pop)
// My classes:
#include "Types.h"
#include <string>
#include "SvmClassifier.h"
#define _CRT_SECURE_NO_WARNINGS

using namespace std;
using namespace fakeit;

class SvmClassifierTestFixture {
public:
	MoodsVector moods;
	SuperVectors superVectors;
	void prepareTrainingData()
	{
		for (int i = 0; i < 100; ++i)
		{
			moods.push_back(0);
			moods.push_back(1);
			moods.push_back(2);
			moods.push_back(3);
	
			superVectors.push_back((cv::Mat_<double>(2, 1) << 1, 0));
			superVectors.push_back((cv::Mat_<double>(2, 1) << 1, 1));
			superVectors.push_back((cv::Mat_<double>(2, 1) << 0, 1));
			superVectors.push_back((cv::Mat_<double>(2, 1) << 0, 0));
		}
	}
};

BOOST_FIXTURE_TEST_SUITE(SvmClassifierTest, SvmClassifierTestFixture)
BOOST_AUTO_TEST_CASE(SimpleTrainingAndPredicting)
{
	prepareTrainingData();
	
	SvmClassifier svmClassifier("anyName");
	svmClassifier.trainSvm(moods, superVectors);

	Mood mood;
	SuperVector superVector;

	//superVector = (cv::Mat_<double>(2, 1) << 0, 1);
	//mood = svmClassifier.predict(superVector);
	//BOOST_CHECK_EQUAL(mood, 2);

	//superVector = (cv::Mat_<double>(2, 1) << 1, 1);
	//mood = svmClassifier.predict(superVector);
	//BOOST_CHECK_EQUAL(mood, 1);

	//superVector = (cv::Mat_<double>(2, 1) << .9, .9);
	//mood = svmClassifier.predict(superVector);
	//BOOST_CHECK_EQUAL(mood, 1);

	//superVector = (cv::Mat_<double>(2, 1) << 0, 0);
	//mood = svmClassifier.predict(superVector);
	//BOOST_CHECK_EQUAL(mood, 3);

	//superVector = (cv::Mat_<double>(2, 1) << 0.4, -0.1);
	//mood = svmClassifier.predict(superVector);
	//BOOST_CHECK_EQUAL(mood, 3);
}
BOOST_AUTO_TEST_SUITE_END()