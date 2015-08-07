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
	virtual void trainSvm(MoodsVector moods, SuperVectors superVectors);
	virtual Mood predict(SuperVector superVector);
	virtual ~SvmClassifier() {
		svm_.clear();
	}
private:
	CvSVM svm_;
};




