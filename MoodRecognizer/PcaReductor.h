#pragma once
#include "Types.h"
#include <opencv2/ml/ml.hpp>
#include "opencv/cv.h"
using namespace cv;

class PcaReductor
{
public:
	PcaReductor(std::vector<int> nOfComponents) {
		nOfComponents_ = nOfComponents;
	}
	virtual void trainPca(SuperVectors superVectors);


	virtual SuperVector reduce(SuperVector superVectorToReduce);
private: 
	std::vector<int> nOfComponents_;
	int getSuperVectorSize(SuperVectors &superVectors);
	PCA pca_;
};

