#include "SuperVectorCalculator.h"
#include "Types.h"

using namespace cv;
SuperVectorCalculator::SuperVectorCalculator(FeatureMatrixLoader& featureMatrixLoader,
	UbmLoader& ubmLoader, vector<Alpha> alpha)
{
	featureMatrixLoader_ = &featureMatrixLoader;
	ubm_ = ubmLoader.getUbm();
	alphas_ = alpha;
}

SuperVectors SuperVectorCalculator::calculate(FileName featureMatrixFileName)
{
	FeatureMatrix featureMatrix = featureMatrixLoader_->get(featureMatrixFileName);
	assert(featureMatrix.rows > 0 && featureMatrix.cols > 0);
	assert(featureMatrix.type() == CV_32FC1);

	int numTimeWindows = featureMatrix.cols;
	int numGaussComponents = ubm_.numGaussComponents_;
	assert(ubm_.weights_.rows == 1);
	assert(ubm_.weights_.cols == numGaussComponents);

	float** eq3 = new float*[numTimeWindows];
	for (int t = 0; t < numTimeWindows; t++)
	{
		float* eq3Counters = new float[numGaussComponents];
		float eq3Denominator = 0;

		for (int componentIdx = 0; componentIdx < numGaussComponents; ++componentIdx)
		{
			float weight = ubm_.weights_.at<float>(componentIdx);
			eq3Counters[componentIdx] = weight * ubm_.logLikelihood(featureMatrix.col(t), componentIdx);
			eq3Denominator += eq3Counters[componentIdx];
		}

		eq3[t] = new float[numGaussComponents];
		for (int componentIdx = 0; componentIdx < numGaussComponents; ++componentIdx)
		{
			assert(abs(eq3Denominator) > 1e-10);
			eq3[t][componentIdx] = eq3Counters[componentIdx] / eq3Denominator;
		}
		delete[] eq3Counters;
	}

	SuperVectors superVectors(alphas_.size());
	//superVectors.resize(alphas_.size(), NULL);
	//SuperVector superVector;
	int numCoeff = featureMatrix.rows;
	for (int componentIdx = 0; componentIdx < numGaussComponents; ++componentIdx)
	{
		Mat eq2Counter = Mat::zeros(numCoeff, 1, CV_32FC1);
		float eq2Denominator = 0;
		for (int t = 0; t < numTimeWindows; t++) {
			eq2Counter += eq3[t][componentIdx] * featureMatrix.col(t);
			eq2Denominator += eq3[t][componentIdx];
		}

		bool eq2DenominatorGreaterThanZero = abs(eq2Denominator) > 1e-10;
		assert(eq2DenominatorGreaterThanZero);
		Mat eq2 = eq2Counter / eq2Denominator;
		assert(eq2.rows == numCoeff);
		assert(eq2.cols == 1);
		assert(ubm_.means_.rows == numCoeff);
		assert(ubm_.means_.cols == numGaussComponents);

		assert(eq2.type() == CV_32FC1);
		// Eq. 1
		int alphaIdx = 0;
		for (auto alpha : alphas_) {
			Mat mu_i;
			addWeighted(eq2, alpha, ubm_.means_.col(componentIdx), 1.0 - alpha, 0.0, mu_i);
			assert(mu_i.rows == numCoeff);
			assert(mu_i.cols == 1);
			appendAdaptedMeanToSuperVector(superVectors[alphaIdx], mu_i);
			++alphaIdx;
		}
	}

	for (int t = 0; t < numTimeWindows; t++)
		delete[] eq3[t];
	delete[] eq3;

	//superVectors.push_back(superVector);

	assert(superVectors.size() == alphas_.size());
	return superVectors;
}


// private:
void SuperVectorCalculator::appendAdaptedMeanToSuperVector(SuperVector &superVector, Mat &mu_i)
{
	int initialNumRows = superVector.rows;
	if (initialNumRows == 0) {
		superVector = mu_i.clone();
	}
	else {
		vconcat(superVector, mu_i, superVector);
	}
	assert(superVector.rows == initialNumRows + mu_i.rows);
	assert(superVector.cols == 1);
}


