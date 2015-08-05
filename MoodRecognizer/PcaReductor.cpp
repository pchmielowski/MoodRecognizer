#include "PcaReductor.h"

void PcaReductor::trainPca(SuperVectors superVectors)
{
	assert(superVectors.size() > 0);

	int numSuperVectors = superVectors.size();
	int sizeSuperVector = getSuperVectorSize(superVectors);

	Mat matrixOfSuperVectors(sizeSuperVector, numSuperVectors, superVectors[0].type());
	for (int i = 0; i < numSuperVectors; i++) {
		// to do : clone
		superVectors[i].convertTo(matrixOfSuperVectors.col(i), matrixOfSuperVectors.type());
	}

	//int sizeAfterPca = sizeSuperVector;
	pca_ = PCA(matrixOfSuperVectors, Mat(), CV_PCA_DATA_AS_COL);

}


SuperVector PcaReductor::reduce(SuperVector superVectorToReduce)
{
	assert(superVectorToReduce.cols == 1);
	assert(superVectorToReduce.rows > 0);

	assert(nOfComponents_.size() > 0);
	int sizeAfterPca = nOfComponents_[0];

	Mat projectedSuperVector = pca_.project(superVectorToReduce);

	double firstElemtentBeforeResizing = projectedSuperVector.at<double>(0);
	projectedSuperVector.resize(sizeAfterPca);
	double firstElemtentAfterResizing = projectedSuperVector.at<double>(0);

	assert(firstElemtentBeforeResizing == firstElemtentAfterResizing);
	assert(projectedSuperVector.cols == 1);
	assert(projectedSuperVector.rows == sizeAfterPca);

	return projectedSuperVector;
}

int PcaReductor::getSuperVectorSize(SuperVectors &superVectors)
{
	assert(superVectors[0].cols == 1);
	if (superVectors.size() > 1)
		assert(superVectors[1].cols == 1);

	return superVectors[0].rows;
}

