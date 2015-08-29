#pragma once
#include <vector>
#include "Types.h"
#include "UbmLoader.h"
#include "FeatureMatrixLoader.h"
#include <boost/math/distributions/normal.hpp>
#include "opencv/cv.h"
#include "Ubm.h"
#include "PlotFilePreparator.h"
#define MAX_NUM_TIME_WINDOWS 100000

typedef float FloatingPoint;
typedef std::vector<FloatingPoint> FloatingPointVector;

using namespace std;
using namespace cv;

class SuperVectorCalculator
{
public:
	SuperVectorCalculator(FeatureMatrixLoader& featureMatrixLoader, UbmLoader& ubmLoader);
	virtual SuperVector calculate(FileName featureMatrixFileName);

	void ReleaseEq3(int numGaussComponents, FloatingPointVector* eq3);

	FloatingPointVector* Eq3(int numTimeWindows, int numGaussComponents, FeatureMatrix &featureMatrix);

	Mat Eq1(Mat eq2, Alpha alpha, int componentIdx, int numCoeff);

	Mat Eq2(int numCoeff, int numTimeWindows, FloatingPointVector* eq3, int componentIdx, FeatureMatrix &featureMatrix, int numGaussComponents, float& probabilisticCount);

private:
	Ubm ubm_;
	FeatureMatrixLoader* featureMatrixLoader_;
	void appendAdaptedMeanToSuperVector(SuperVector &superVector, Mat &mu_i);

};
