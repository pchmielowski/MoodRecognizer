#include "MatFeatureMatrixLoader.h"
#include "matio.h"
#include <opencv\cv.h>
#include <iostream>

using namespace std;
using namespace cv;

MatFeatureMatrixLoader::MatFeatureMatrixLoader(const bool sdcEnabled)
{

}

FeatureMatrix MatFeatureMatrixLoader::get(const FileName fileName)
{
	cout << fileName << endl;
	cv::FileStorage file;
	file.open(fileName, cv::FileStorage::READ);

	Mat oneMfcc;
	file["Mfcc"] >> oneMfcc;
	
	file.release();
	return oneMfcc;
}
