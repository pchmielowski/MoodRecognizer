#include "MatFeatureMatrixLoader.h"
#include "matio.h"
using namespace std;
MatFeatureMatrixLoader::MatFeatureMatrixLoader(const bool sdcEnabled)
{

}

FeatureMatrix MatFeatureMatrixLoader::get(const FileName fileName)
{

}

Mat readMfccFromMatFile(const FileName fileName) {
	// open MAT file
	mat_t* inputFile = NULL;
	inputFile = Mat_Open(fileName.c_str(), MAT_ACC_RDONLY);
	if (!inputFile) {
		throw(runtime_error("MAT file not valid!"));
	}

	// get MFCC features from MAT file
	matvar_t* featuresMatStructure = NULL;
	featuresMatStructure = Mat_VarRead(inputFile, "features");
	matvar_t* mfccMatVector = NULL;
	mfccMatVector = Mat_VarGetStructField(featuresMatStructure, "ceps", MAT_BY_NAME, NULL);

	int dimMatrix[2] = { *(mfccMatVector->dims), *(mfccMatVector->dims + 1) };
	int numSamples = dimMatrix[0] * dimMatrix[1];
	double* dataAsVectorOfDouble = new double[numSamples];
	int startPoint[2] = { 0, 0 };
	int step[2] = { 1, 1 };
	Mat_VarReadData(inputFile, mfccMatVector, dataAsVectorOfDouble, startPoint, step, dimMatrix);
	Mat_VarFree(featuresMatStructure);
	Mat_Close(inputFile);


	float* dataAsVectorOfFloat = new float[numSamples];
	for (int i = 0; i < numSamples; i++)
		dataAsVectorOfFloat[i] = static_cast<float>(dataAsVectorOfDouble[i]);
	delete[] dataAsVectorOfDouble;
	
	// ta macierz jest transponowana w stosunku do macierzy z Matlaba!!
	Mat oneMfcc = Mat(numFramesAfterReduce, dimMatrix[0], CV_32F, dataAsVectorOfFloat).clone();
	delete[] dataAsVectorOfFloat;
	return oneMfcc;
}