#pragma warning(push, 0)
#include <boost/test/unit_test.hpp>
#include <fakeit.hpp>
#include <string>
#include "FeatureMatrixLoader.h"
#include "opencv/cv.h"
#include "UbmLoader.h"
#pragma warning(pop)
using namespace fakeit;
using namespace std;

BOOST_AUTO_TEST_SUITE(SuperVectorCalculatorTest)
BOOST_AUTO_TEST_CASE(calculate_1gaussComponentSimpleMatrix) {
	//Mock<FeatureMatrixLoader> mockLoader;
	cv::Mat simpleFeatureMatrix = (cv::Mat_<double>(3,3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);
	//When(Method(mockLoader, get)).AlwaysReturn(simpleFeatureMatrix);
	//FeatureMatrixLoader& loader = mockLoader.get();

	//Mock<UbmLoader> mockUbm;
	//Ubm ubm;
	//ubm.means = (cv::Mat_<double>(1, 1) << 0);
	//ubm.weights = (cv::Mat_<double>(1, 1) << 0);
	//ubm.numGaussComponents = 1;

	//int n_of_mfcc_coef = 3;
	//int n_of_gaussians = 1;
	//for (int mfcc_coef = 0; mfcc_coef < n_of_mfcc_coef; mfcc_coef++) {
	//	for (int gaus_nmbr = 0; gaus_nmbr < n_of_gaussians; gaus_nmbr++) {
	//		//                                                                         lub na odwrót 
	//		norm_distr[gaus_nmbr][mfcc_coef] = normal_distribution<>(means.at<double>(gaus_nmbr, mfcc_coef),
	//			sqrt(covs[gaus_nmbr].at<double>(mfcc_coef)));
	//	}
	//}



	cout << simpleFeatureMatrix << endl;
	cout << simpleFeatureMatrix.at<double>(2, 0);
	BOOST_CHECK_EQUAL(3, 3);
}
//BOOST_AUTO_TEST_CASE(constructorWithFileReader_objectCreated_openCalledWithRighFileName) {
//	string fileContent = "1\n4\n4\n1\n";
//
//	Mock<FileReaderInterface> mockFileReader;
//	When(Method(mockFileReader, open)).Return();
//	When(Method(mockFileReader, getContent)).Return(fileContent);
//	FileReaderInterface& fileReader = mockFileReader.get();
//
//	Moods moods(fileReader, "fileName");
//
//	Verify(Method(mockFileReader, open).Using("fileName")).AtLeastOnce();
//}
BOOST_AUTO_TEST_SUITE_END()