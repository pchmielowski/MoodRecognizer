#pragma once;
#include "Types.h"
#include "MoodsInterface.h"
#include "opencv2\ml\ml.hpp"
#include "opencv/cv.h"

using namespace cv;

class SvmClassifier
{
public:
	SvmClassifier(FileName svmModelFileName);
	virtual float trainSvm(const MoodsVector moods, SuperVectors superVectors);

	virtual Mood predict(SuperVector superVector);
	virtual ~SvmClassifier() {
		svm_.clear();
	}
private:
	CvSVM svm_;
	float computeAccuracy(Mat& superVectorsAsMat, Mat &moodsAsMat);
	Mat createSuperVectorMatrix(SuperVectors &superVectors);

	Mat createMoodsMatrix(MoodsVector &moods);
	void divideInTestAndTrainSubsets(MoodsVector &moods, MoodsVector &testMoods, SuperVectors &testSuperVectors, SuperVectors &superVectors);
};




