#pragma once
#include "Types.h"

class FeatureMatrixLoader
{
public:
	FeatureMatrixLoader() {};
	virtual FeatureMatrix get(const FileName fileName) = 0;
	
private:
};
