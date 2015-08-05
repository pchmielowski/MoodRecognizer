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
	void trainSvm(MoodsVector moods, SuperVectors superVectors);
	Mood predict(SuperVector superVector);
	~SvmClassifier() {
		svm_.clear();
	}
private:
	CvSVM svm_;
};




