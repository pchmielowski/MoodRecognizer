#include "SvmClassifier.h"
#include "opencv2\ml\ml.hpp"
#include "opencv/cv.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace cv;

float SvmClassifier::trainSvm(MoodsVector moods, SuperVectors superVectors)
{
	if (moods.size() != superVectors.size())
		throw std::runtime_error("Number of moods != number of sVectors!");

	srand(time(NULL));
	MoodsVector testMoods;
	SuperVectors testSuperVectors;
	int sizeTestSet = int(float(moods.size())*.8f);
	for (int i = 0; i < sizeTestSet; ++i)
	{
		int toMoveIdx = rand() % moods.size();
		testMoods.push_back(moods[toMoveIdx]);
		moods.erase(moods.begin() + toMoveIdx);
		testSuperVectors.push_back(superVectors[toMoveIdx]);
		superVectors.erase(superVectors.begin() + toMoveIdx);
	}

	Mat moodsAsMat = createMoodsMatrix(moods);
	Mat superVectorsAsMat = createSuperVectorMatrix(superVectors);
	Mat t_moodsAsMat = createMoodsMatrix(testMoods);
	Mat t_superVectorsAsMat = createSuperVectorMatrix(testSuperVectors);

	assert(superVectorsAsMat.type() == CV_32FC1);
	assert(superVectorsAsMat.cols == superVectors[0].rows);
	assert(superVectorsAsMat.rows == moodsAsMat.rows);
	assert(superVectorsAsMat.rows == superVectors.size());

	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::RBF;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	const int numFolds = 5;
	svm_.train_auto(superVectorsAsMat, moodsAsMat, Mat(), Mat(), params, numFolds);

	float accuracy = computeAccuracy(t_superVectorsAsMat, t_moodsAsMat);
	return accuracy;
}



float SvmClassifier::computeAccuracy(Mat& superVectorsAsMat, Mat &moodsAsMat)
{
	Mat results;
	svm_.predict(superVectorsAsMat, results);
#if 0
	for (int i = 0; i < results.rows; ++i)
		results.at<float>(i) = rand() % 4;
#endif
	int numCorrectPredicts = countNonZero(results != moodsAsMat);
	return (float)numCorrectPredicts / (float)(moodsAsMat.rows);
}

cv::Mat SvmClassifier::createSuperVectorMatrix(SuperVectors &superVectors)
{
	if (superVectors[0].type() != CV_32FC1)
		throw std::runtime_error("Type of superVector is not CV_32FC1!");

	Mat superVectorsAsMat;
	for (auto superVector : superVectors)
	{
		if (superVectorsAsMat.empty())
			superVectorsAsMat = superVector.t();
		else
			vconcat(superVectorsAsMat, superVector.t(), superVectorsAsMat);
		superVector.release();
	}
	return superVectorsAsMat;
}

cv::Mat SvmClassifier::createMoodsMatrix(MoodsVector &moods)
{
	Mat moodsAsMat(moods.size(), 1, CV_32FC1);
	int moodIdx = 0;
	for (auto mood : moods) {
		moodsAsMat.at<float>(moodIdx++) = static_cast<float>(mood);
	}

	assert(moodsAsMat.type() == CV_32FC1);
	assert(moodsAsMat.rows == moods.size());

	return moodsAsMat;
}

SvmClassifier::SvmClassifier(FileName svmModelFileName)
{

}

Mood SvmClassifier::predict(SuperVector superVector)
{
	if (superVector.type() != CV_32FC1)
		throw std::runtime_error("Type of superVector is not CV_32FC1!");
	if (superVector.cols != 1)
		throw std::runtime_error("SuperVector has more than 1 column!");

	superVector = superVector.t();
	assert(superVector.rows == 1);

	int mood = static_cast<int>(svm_.predict(superVector));
	assert(mood >= 0 && mood <= 3);
	return mood;
}