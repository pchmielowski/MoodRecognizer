#include "SvmClassifier.h"
#include "opencv2\ml\ml.hpp"
#include "opencv/cv.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace cv;

float SvmClassifier::trainSvm(const MoodsVector moods, SuperVectors superVectors)
{
	if (moods.size() != superVectors.size())
		throw std::runtime_error("Number of moods != number of sVectors!");

	normalize(superVectors);

	srand(time(NULL));
	const int NUM_FOLDS_OUTER = 50;
	float sumAccuracy = 0;
	CvSVMParams params;
	for (int foldIdx = 0; foldIdx < NUM_FOLDS_OUTER; ++foldIdx)
	{
		MoodsVector		testMoods;
		SuperVectors	testSuperVectors;
		MoodsVector		trainMoods = moods;
		SuperVectors	trainSuperVectors = superVectors;
		divideInTestAndTrainSubsets(trainMoods, testMoods, testSuperVectors, trainSuperVectors);

		Mat moodsAsMat			= createMoodsMatrix(trainMoods);
		Mat superVectorsAsMat	= createSuperVectorMatrix(trainSuperVectors);
		Mat t_moodsAsMat		= createMoodsMatrix(testMoods);
		Mat t_superVectorsAsMat = createSuperVectorMatrix(testSuperVectors);

		assert(superVectorsAsMat.type() == CV_32FC1);
		assert(superVectorsAsMat.cols == trainSuperVectors[0].rows);
		assert(superVectorsAsMat.rows == moodsAsMat.rows);
		assert(superVectorsAsMat.rows == trainSuperVectors.size());

		const int NUM_FOLDS_INNER = 10;
		bool isFirstIteration = foldIdx == 0;
		if (isFirstIteration)
		{
			params.svm_type = CvSVM::C_SVC;
			params.kernel_type = CvSVM::RBF;
			params.term_crit = cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10000, 1e-6);
			svm_.train_auto(superVectorsAsMat, moodsAsMat, Mat(), Mat(), params, NUM_FOLDS_INNER);
			params = svm_.get_params();
		}
		else
		{
			svm_.train(superVectorsAsMat, moodsAsMat, Mat(), Mat(), params);
		}
		//std::cout << "Next SVM trained" << std::endl;
		sumAccuracy += computeAccuracy(t_superVectorsAsMat, t_moodsAsMat);
	}
	return sumAccuracy / static_cast<float>(NUM_FOLDS_OUTER);
}



float SvmClassifier::computeAccuracy(Mat& superVectorsAsMat, Mat &moodsAsMat)
{
	Mat results;
	svm_.predict(superVectorsAsMat, results);
#if 0
	for (int i = 0; i < results.rows; ++i)
		results.at<float>(i) = rand() % 4;
#endif
	int numCorrectPredicts = countNonZero(results == moodsAsMat);
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

void SvmClassifier::divideInTestAndTrainSubsets(MoodsVector &moods, MoodsVector &testMoods, SuperVectors &testSuperVectors, SuperVectors &superVectors)
{
	int sizeTestSet = int(float(moods.size())*.8f);
	for (int j = 0; j < sizeTestSet; ++j)
	{
		int toMoveIdx = rand() % moods.size();
		//std::cout << toMoveIdx << " ";
		testMoods.push_back(moods[toMoveIdx]);
		moods.erase(moods.begin() + toMoveIdx);
		testSuperVectors.push_back(superVectors[toMoveIdx]);
		superVectors.erase(superVectors.begin() + toMoveIdx);
	}
	//std::cout << std::endl;
}

void SvmClassifier::normalize(SuperVectors &superVectors) // TODO: refactor
{
	int superVectorSize = superVectors[0].rows;
	// make values fall into <-1, 1>
	vector<float> absMax;
	absMax.resize(superVectorSize, 0.f);
	for (auto superVector : superVectors)
	{
		for (int dimIdx = 0; dimIdx < absMax.size(); ++dimIdx)
		{
			float currentAbs = abs(superVector.at<float>(dimIdx));
			if (currentAbs > absMax[dimIdx])
				absMax[dimIdx] = currentAbs;
		}
	}
	for (auto superVector : superVectors)
	{
		for (int dimIdx = 0; dimIdx < absMax.size(); ++dimIdx)
		{
			superVector.at<float>(dimIdx) /= absMax[dimIdx];
		}
	}
	// make means = 0
	vector<float> sums;
	sums.resize(superVectorSize, 0.f);
	for (auto superVector : superVectors)
	{
		for (int dimIdx = 0; dimIdx < absMax.size(); ++dimIdx)
		{
			sums[dimIdx] = superVector.at<float>(dimIdx);
		}
	}
	for (auto superVector : superVectors)
	{
		for (int dimIdx = 0; dimIdx < absMax.size(); ++dimIdx)
		{
			float mean = sums[dimIdx] / static_cast<float>(superVectorSize);
			superVector.at<float>(dimIdx) -= mean;
		}
	}
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