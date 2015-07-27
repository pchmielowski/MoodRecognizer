#pragma once
#include "Types.h"

class FeatureMatrixLoader
{
public:
	FeatureMatrixLoader(bool sdcEnabled);
	FeatureMatrix get(FileName fileName);
	
private:
};

FeatureMatrix FeatureMatrixLoader::get(FileName fileName)
{
	return FeatureMatrix();
}
