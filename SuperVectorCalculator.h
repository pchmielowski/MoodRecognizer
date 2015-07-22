#pragma once
#include <vector>
#include "Types.h"

using namespace std;

class FeatureMatrixLoader;
class Ubm;

class SuperVectorCalculator
{
public:
	SuperVectorCalculator(FeatureMatrixLoader& featureMatrixLoader, vector<Alpha> alpha, vector<int> numComponents, Ubm& ubm);
	SuperVector calculate(FileName fileName);
private:
};
