#include "MatFeatureMatrixLoader.h"
#include "matio.h"
#include <opencv\cv.h>

using namespace std;
using namespace cv;

MatFeatureMatrixLoader::MatFeatureMatrixLoader(const bool sdcEnabled)
{

}

FeatureMatrix MatFeatureMatrixLoader::get(const FileName fileName)
{
	//cout << fileName << endl;
	mat_t* inputFile = openMatFile(fileName);

	Mat oneMfcc = getMatrix(inputFile);
	
	return oneMfcc.t();
}

cv::Mat MatFeatureMatrixLoader::getMatrix(mat_t* inputFile)
{
	// getCeps
	matvar_t* featuresMatStructure = NULL;
	featuresMatStructure = Mat_VarRead(inputFile, "features");
	matvar_t* mfccMatVector = NULL;
	mfccMatVector = Mat_VarGetStructField(featuresMatStructure, "ceps", MAT_BY_NAME, 0);

	int dimMatrix[2] = { *(mfccMatVector->dims), *(mfccMatVector->dims + 1) };
	int numSamples = dimMatrix[0] * dimMatrix[1];
	double* dataAsVectorOfDouble = new double[numSamples];
	int startPoint[2] = { 0, 0 };
	int step[2] = { 1, 1 };
	Mat_VarReadData(inputFile, mfccMatVector, dataAsVectorOfDouble, startPoint, step, dimMatrix);
	Mat_VarFree(featuresMatStructure);
	Mat_Close(inputFile);

	// castToFloat
	float* dataAsVectorOfFloat = new float[numSamples];
	for (int i = 0; i < numSamples; i++)
		dataAsVectorOfFloat[i] = static_cast<float>(dataAsVectorOfDouble[i]);
	delete[] dataAsVectorOfDouble;

	// castToMat
	// ta macierz jest transponowana w stosunku do macierzy z Matlaba!!
	int dimMatrixTransposed[2] = { dimMatrix[1], dimMatrix[0] };
	Mat oneMfcc = Mat(2, dimMatrixTransposed, CV_32F, dataAsVectorOfFloat).clone();
	delete[] dataAsVectorOfFloat;

	return oneMfcc.clone();
}

mat_t* MatFeatureMatrixLoader::openMatFile(const FileName &fileName)
{
	mat_t* inputFile = NULL;
	inputFile = Mat_Open(fileName.c_str(), MAT_ACC_RDONLY);
	if (!inputFile) {
		throw(runtime_error("MAT file not valid!"));
	}
	return inputFile;
}
