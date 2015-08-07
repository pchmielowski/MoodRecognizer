#include "SvmClassifier.h"
#include "opencv2\ml\ml.hpp"
#include "opencv/cv.h"
#include <iostream>

using namespace cv;

void SvmClassifier::trainSvm(MoodsVector moods, SuperVectors superVectors)
{
	if (moods.size() != superVectors.size())
		throw std::runtime_error("Number of moods != number of sVectors!");

	Mat moodsAsMat(moods.size(), 1, CV_32FC1);
	int moodIdx = 0;
	for (auto mood : moods) {
		moodsAsMat.at<float>(moodIdx++) = static_cast<float>(mood);
	}

	//moodsAsMat.convertTo(moodsAsMat, CV_32FC1); // TODO: usun¹æ przy optymalizacji
	assert(moodsAsMat.type() == CV_32FC1);
	assert(moodsAsMat.rows == moods.size());

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
	//superVectorsAsMat.convertTo(superVectorsAsMat, CV_32FC1);  // TODO: usun¹æ przy optymalizacji
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

SvmClassifier::SvmClassifier(FileName svmModelFileName)
{

}

