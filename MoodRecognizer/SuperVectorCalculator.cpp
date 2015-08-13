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

	float** eq3 = Eq3(numTimeWindows, numGaussComponents, featureMatrix);

	SuperVectors superVectors(alphas_.size());
	int numCoeff = featureMatrix.rows;
	for (int componentIdx = 0; componentIdx < numGaussComponents; ++componentIdx)
	{
		Mat eq2 = Eq2(numCoeff, numTimeWindows, eq3, componentIdx, featureMatrix, numGaussComponents);
		int alphaIdx = 0;
		for (auto alpha : alphas_) {
			Mat mu_i = Eq1(eq2, alpha, componentIdx, numCoeff);
			appendAdaptedMeanToSuperVector(superVectors[alphaIdx], mu_i);
			++alphaIdx;
		}
	}
	ReleaseEq3(numTimeWindows, eq3);

	assert(superVectors.size() == alphas_.size());
	return superVectors;
}

void SuperVectorCalculator::ReleaseEq3(int numTimeWindows, float** eq3)
{
	for (int t = 0; t < numTimeWindows; t++)
	{
		assert(eq3[t] != nullptr);
		delete[] eq3[t];
	}
	delete[] eq3;
}

float** SuperVectorCalculator::Eq3(int numTimeWindows, int numGaussComponents, FeatureMatrix &featureMatrix)
{
	float** eq3 = new float*[numTimeWindows];
	for (int t = 0; t < numTimeWindows; t++)
	{
		float* eq3Counters = new float[numGaussComponents];
		float eq3Denominator = 0;

		for (int componentIdx = 0; componentIdx < numGaussComponents; ++componentIdx)
		{
			float weight = ubm_.weights_.at<float>(componentIdx);
			eq3Counters[componentIdx] = weight * ubm_.logLikelihood(featureMatrix.col(t), componentIdx);
			if (std::isnan(eq3Counters[componentIdx]))
				throw runtime_error("One of Eq3 counters is NaN!");
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
	return eq3;
}

cv::Mat SuperVectorCalculator::Eq1(Mat eq2, Alpha alpha, int componentIdx, int numCoeff)
{
	Mat mu_i;
	addWeighted(eq2, alpha, ubm_.means_.col(componentIdx), 1.0 - alpha, 0.0, mu_i);
	assert(mu_i.rows == numCoeff);
	assert(mu_i.cols == 1);
	return mu_i;
}

cv::Mat SuperVectorCalculator::Eq2(int numCoeff, int numTimeWindows, float** eq3, int componentIdx, FeatureMatrix &featureMatrix, int numGaussComponents)
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
	return eq2;
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


