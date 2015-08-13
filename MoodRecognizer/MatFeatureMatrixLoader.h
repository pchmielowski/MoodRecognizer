#pragma once
#include "Types.h"
#include "FeatureMatrixLoader.h"
#include "matio.h"
#include <opencv\cv.h>

using namespace std;
using namespace cv;


class MatFeatureMatrixLoader : public FeatureMatrixLoader
{
public:
	MatFeatureMatrixLoader(const bool sdcEnabled);
	FeatureMatrix get(const FileName fileName) override;


private:
};
