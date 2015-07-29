#pragma once
#include "Types.h"
#include "FeatureMatrixLoader.h"

class MatFeatureMatrixLoader : public FeatureMatrixLoader
{
public:
	MatFeatureMatrixLoader(const bool sdcEnabled);
	FeatureMatrix get(const FileName fileName) override;
private:
};
