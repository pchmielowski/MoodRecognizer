#pragma once
#include <vector>
#include "Types.h"
#include "UbmLoader.h"
#include "FeatureMatrixLoader.h"
#include <boost/math/distributions/normal.hpp>
#include "opencv/cv.h"
#include "Ubm.h"
#include "PlotFilePreparator.h"

using namespace std;
using namespace cv;

class SuperVectorCalculator
{
public:
	SuperVectorCalculator(FeatureMatrixLoader& featureMatrixLoader, UbmLoader& ubmLoader, 
		vector<Alpha> alpha);
	virtual SuperVectors calculate(FileName featureMatrixFileName);

	void ReleaseEq3(int numTimeWindows, float** eq3);

	float** Eq3(int numTimeWindows, int numGaussComponents, FeatureMatrix &featureMatrix);

	Mat Eq1(Mat eq2, Alpha alpha, int componentIdx, int numCoeff);

	Mat Eq2(int numCoeff, int numTimeWindows, float** eq3, int componentIdx, FeatureMatrix &featureMatrix, int numGaussComponents);

	void addAlphasToWriter(PlotFilePreparator& plotFilePreparator)
	{
		plotFilePreparator.addAlphas(alphas_);
	}
private:
	Ubm ubm_;
	vector<Alpha> alphas_;
	FeatureMatrixLoader* featureMatrixLoader_;
	void appendAdaptedMeanToSuperVector(SuperVector &superVector, Mat &mu_i);

};
