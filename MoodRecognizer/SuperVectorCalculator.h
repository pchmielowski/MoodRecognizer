#pragma once
#include <vector>
#include "Types.h"
#include "Ubm.h"
#include "FeatureMatrixLoader.h"

using namespace std;

class SuperVectorCalculator
{
public:
	SuperVectorCalculator(FeatureMatrixLoader& featureMatrixLoader, 
		vector<Alpha> alpha, vector<int> numComponents, Ubm& ubm);
	SuperVector calculate(FileName fileName);
private:
	Ubm* ubm_;
	FeatureMatrixLoader* featureMatrixLoader_;
};
