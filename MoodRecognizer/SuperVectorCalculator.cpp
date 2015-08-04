#include "SuperVectorCalculator.h"
#include "Types.h"

using namespace cv;

SuperVector SuperVectorCalculator::calculate(FileName featureMatrixFileName)
{
	//ubmLoader_
	FeatureMatrix featureMatrix = featureMatrixLoader_->get(featureMatrixFileName);
	assert(featureMatrix.rows > 0 && featureMatrix.cols > 0);
	int numTimeWindows = featureMatrix.cols;
	int numGaussComponents = ubm_.numGaussComponents;

	double** eq3 = new double*[numTimeWindows];
	for (int t = 0; t < numTimeWindows; t++)
	{
		double* eq3Counters = new double[numGaussComponents];
		double eq3Denominator = 0;

		for (int componentIdx = 0; componentIdx < numGaussComponents; ++componentIdx)
		{
			double weight = ubm_.weights.at<double>(componentIdx);
			eq3Counters[componentIdx] = weight * ubm_.logLikelihood(featureMatrix.col(t), componentIdx);
			eq3Denominator += eq3Counters[componentIdx];
			// TODO: podczas optymalizacji sprawdziæ
			// czy da siê omin¹æ =+, 
			// ¿eby b³êdy numeryczne siê nie akumulowa³y tak bardzo
		}

		eq3[t] = new double[numGaussComponents];
		for (int componentIdx = 0; componentIdx < numGaussComponents; ++componentIdx)
		{
			eq3[t][componentIdx] = eq3Counters[componentIdx] / eq3Denominator;
		}
		delete[] eq3Counters;
	}

	SuperVector superVector;
	int alphaIdx = 0;
	int numCoeff = featureMatrix.rows;
	for (int componentIdx = 0; componentIdx < numGaussComponents; ++componentIdx)
	{
		Mat eq2Counter = Mat::zeros(1, numCoeff, CV_64F);
		double eq2Denominator = 0;
		for (int t = 0; t < numTimeWindows; t++) {
			eq2Counter += eq3[t][componentIdx] * featureMatrix.col(t);
			eq2Denominator += eq3[t][componentIdx];
		}

		Mat eq2 = eq2Counter / eq2Denominator;
		assert(eq2.rows == numCoeff);
		assert(eq2.cols == 1);
		assert(ubm_.means.rows == numCoeff);
		assert(ubm_.means.cols == numGaussComponents);

		// Eq. 1
		Mat mu_i;
		addWeighted(eq2, alphas_[alphaIdx], ubm_.means.col(componentIdx), 1.0 - alphas_[alphaIdx], 0.0, mu_i);
		assert(mu_i.rows == numCoeff);
		assert(mu_i.cols == 1);
		appendAdaptedMeanToSuperVector(superVector, mu_i);
	}

	for (int t = 0; t < numTimeWindows; t++)
		delete[] eq3[t];
	delete[] eq3;
	return superVector;
}

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

SuperVectorCalculator::SuperVectorCalculator(FeatureMatrixLoader& featureMatrixLoader, vector<Alpha> alpha, vector<int> numComponents, UbmLoader& ubm)
{

}

