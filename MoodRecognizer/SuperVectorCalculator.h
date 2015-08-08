#pragma once
#include <vector>
#include "Types.h"
#include "UbmLoader.h"
#include "FeatureMatrixLoader.h"
#include <boost/math/distributions/normal.hpp>
#include "opencv/cv.h"
#include "Ubm.h"

using namespace std;
using namespace cv;

class SuperVectorCalculator
{
public:
	SuperVectorCalculator(FeatureMatrixLoader& featureMatrixLoader, UbmLoader& ubmLoader, 
		vector<Alpha> alpha);
	virtual SuperVectors calculate(FileName featureMatrixFileName);
private:
	Ubm ubm_;
	vector<Alpha> alphas_;
	FeatureMatrixLoader* featureMatrixLoader_;
	void appendAdaptedMeanToSuperVector(SuperVector &superVector, Mat &mu_i);

};
