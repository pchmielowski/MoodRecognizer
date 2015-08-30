#include "PcaReductor.h"
const float REDUCE_SUPER_VECTOR_FACTOR = 1.5;

void PcaReductor::trainPca(SuperVectors superVectors)
{
	assert(superVectors.size() > 0);

	int numSuperVectors = superVectors.size();
	int sizeSuperVector = getSuperVectorSize(superVectors);
	// REDUCE BEFORE PCA
	int reducedSize = sizeSuperVector / REDUCE_SUPER_VECTOR_FACTOR;
	//

	Mat matrixOfSuperVectors(reducedSize, numSuperVectors, superVectors[0].type());
	for (int i = 0; i < numSuperVectors; i++) {
		// REDUCE BEFORE PCA
		SuperVector temp = superVectors[i].clone();
		temp.resize(reducedSize, 1);
		// 

		temp.convertTo(matrixOfSuperVectors.col(i), matrixOfSuperVectors.type());
	}

	pca_ = PCA(matrixOfSuperVectors, Mat(), CV_PCA_DATA_AS_COL);
}


SuperVector PcaReductor::reduce(SuperVector superVectorToReduce)
{
	assert(superVectorToReduce.cols == 1);
	assert(superVectorToReduce.rows > 0);

	assert(nOfComponents_.size() > 0);
	int sizeAfterPca = nOfComponents_[0];

	// REDUCE BEFORE PCA
	int sizeSuperVector = superVectorToReduce.rows;
	int reducedSize = sizeSuperVector / REDUCE_SUPER_VECTOR_FACTOR;
	superVectorToReduce.resize(reducedSize, 1);
	// 

	Mat projectedSuperVector = pca_.project(superVectorToReduce);
	float firstElemtentBeforeResizing = projectedSuperVector.at<float>(0);
	projectedSuperVector.resize(sizeAfterPca);
	float firstElemtentAfterResizing = projectedSuperVector.at<float>(0);

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

