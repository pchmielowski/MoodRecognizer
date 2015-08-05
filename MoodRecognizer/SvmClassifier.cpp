#include "SvmClassifier.h"
#include "opencv2\ml\ml.hpp"
#include "opencv/cv.h"
#include <iostream>

using namespace cv;

void SvmClassifier::trainSvm(MoodsVector moods, SuperVectors superVectors)
{
	Mat moodsAsMat;
	for (auto mood : moods)
		moodsAsMat.push_back(mood);
	moodsAsMat.convertTo(moodsAsMat, CV_32FC1); // TODO: usun�� przy optymalizacji
	assert(moodsAsMat.type() == CV_32FC1);

	Mat superVectorsAsMat;
	for (auto superVector : superVectors)
	{
		Mat transposed = superVector.t();
		superVectorsAsMat.push_back(transposed);
	}
	superVectorsAsMat.convertTo(superVectorsAsMat, CV_32FC1);  // TODO: usun�� przy optymalizacji
	assert(superVectorsAsMat.type() == CV_32FC1);

	assert(superVectorsAsMat.rows == moodsAsMat.rows);

	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::RBF;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	const int numFolds = 2;
	svm_.train_auto(superVectorsAsMat, moodsAsMat, Mat(), Mat(), params, numFolds);
}

Mood SvmClassifier::predict(SuperVector superVector)
{
	superVector = superVector.t();
	superVector.convertTo(superVector, CV_32FC1);

	assert(superVector.rows == 1);
	assert(superVector.type() == CV_32FC1);

	float response = svm_.predict(superVector);
	int intResponse = static_cast<int>(response);
	return intResponse;
}

SvmClassifier::SvmClassifier(FileName svmModelFileName)
{

}

