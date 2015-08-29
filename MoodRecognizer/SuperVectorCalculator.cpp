#include "SuperVectorCalculator.h"
#include "Types.h"
#include <iostream>

using namespace cv;
SuperVectorCalculator::SuperVectorCalculator(FeatureMatrixLoader& featureMatrixLoader,
	UbmLoader& ubmLoader)
{
	featureMatrixLoader_ = &featureMatrixLoader;
	ubm_ = ubmLoader.getUbm();
}

SuperVector SuperVectorCalculator::calculate(FileName featureMatrixFileName)
{
	FeatureMatrix featureMatrix = featureMatrixLoader_->get(featureMatrixFileName);
	assert(featureMatrix.rows > 0 && featureMatrix.cols > 0);
	assert(featureMatrix.type() == CV_32FC1);

	int numTimeWindows = featureMatrix.cols;
	int numGaussComponents = ubm_.numGaussComponents_;
	assert(ubm_.weights_.rows == 1);
	assert(ubm_.weights_.cols == numGaussComponents);

	myContainer* eq3 = Eq3(numTimeWindows, numGaussComponents, featureMatrix);

	SuperVector superVector;
	int numCoeff = featureMatrix.rows;
	for (int componentIdx = 0; componentIdx < numGaussComponents; ++componentIdx)
	{
		float probabilisticCount;
		Mat eq2 = Eq2(numCoeff, numTimeWindows, eq3, componentIdx, featureMatrix, numGaussComponents, probabilisticCount);

		Alpha alpha;
		if (probabilisticCount > DBL_MIN) {
			const float RELEVANCE_FACTOR = 14;
			alpha = probabilisticCount / (probabilisticCount + RELEVANCE_FACTOR);
		}
		else {
			alpha = 0.0;
		}

		Mat mu_i = Eq1(eq2, alpha, componentIdx, numCoeff);
		appendAdaptedMeanToSuperVector(superVector, mu_i);
	}
	ReleaseEq3(numGaussComponents, eq3);

	return superVector;
}

void SuperVectorCalculator::ReleaseEq3(int numGaussComponents, myContainer* eq3)
{
	delete[] eq3;
}

myContainer* SuperVectorCalculator::Eq3(int numTimeWindows, int numGaussComponents, FeatureMatrix &featureMatrix)
{
	myContainer* eq3 = new myContainer[numGaussComponents];

	for (int t = 0; t < numTimeWindows; t++)
	{
		double* eq3Counters = new double[numGaussComponents];
		double eq3Denominator = 0;

		for (int componentIdx = 0; componentIdx < numGaussComponents; ++componentIdx)
		{
			//std::cout << "Component number: " << componentIdx << std::endl;
			eq3Counters[componentIdx] = ubm_.weightedLogLikelihood(featureMatrix.col(t), componentIdx);
			assert(abs(eq3Counters[componentIdx]) > DBL_MIN);
			if (std::isnan(eq3Counters[componentIdx]))
				throw runtime_error("One of Eq3 counters is NaN!");
			eq3Denominator += eq3Counters[componentIdx];
		}

		for (int componentIdx = 0; componentIdx < numGaussComponents; ++componentIdx)
		{
			if (!(abs(eq3Counters[componentIdx]) >= DBL_MIN))
				throw(runtime_error("Eq 3 counter =/= 0 for component " + to_string(componentIdx)));
			if (!(abs(eq3Denominator) >= DBL_MIN))
				throw(runtime_error("Eq 3 denominator =/= 0 for component " + to_string(componentIdx)));

			double eq3forComponent = eq3Counters[componentIdx] / eq3Denominator;

			eq3[componentIdx].push_back(eq3forComponent);
		}
		delete[] eq3Counters;
	}
	return eq3;
}

cv::Mat SuperVectorCalculator::Eq2(int numCoeff, int numTimeWindows, myContainer* eq3, int componentIdx, FeatureMatrix &featureMatrix, int numGaussComponents, float& probabilisticCount)
{
	Mat eq2Counter = Mat::zeros(numCoeff, 1, CV_32FC1);
	probabilisticCount = 0;
	int t = 0;
	for (auto itr : eq3[componentIdx])
	{
		eq2Counter += float(itr) * featureMatrix.col(t++);
		probabilisticCount += itr;
	}

	Mat eq2 = eq2Counter / probabilisticCount;
	assert(eq2.rows == numCoeff);
	assert(eq2.cols == 1);
	assert(ubm_.means_.rows == numCoeff);
	assert(ubm_.means_.cols == numGaussComponents);
	assert(eq2.type() == CV_32FC1);
	return eq2;
}

cv::Mat SuperVectorCalculator::Eq1(Mat eq2, Alpha alpha, int componentIdx, int numCoeff)
{
	Mat mu_i;
	addWeighted(eq2, alpha, ubm_.means_.col(componentIdx), 1.0 - alpha, 0.0, mu_i);
	assert(mu_i.rows == numCoeff);
	assert(mu_i.cols == 1);
	return mu_i;
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


