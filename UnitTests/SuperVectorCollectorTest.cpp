#pragma warning(push, 0)
#include <boost/test/unit_test.hpp>
#include <fakeit.hpp>
#include <string>
#include "opencv/cv.h"
#include "UbmLoader.h"
#pragma warning(pop)
//#include "FeatureMatrixLoader.h"
//#include "SuperVectorCalculator.h"
//#include "UbmLoader.h"
#include "SuperVectorCollector.h"
#include "MoodsInterface.h"
#include "InputFileNames.h"
using namespace fakeit;
using namespace std;

class SuperVectorCollectorTestFixture
{
public:
	//Mock<FeatureMatrixLoader> featureMatrixLoader;
	//FeatureMatrixLoader& featureMatrixLoaderFactory(FeatureMatrix& simpleFeatureMatrix) {
	//	When(Method(featureMatrixLoader, get)).AlwaysReturn(simpleFeatureMatrix);
	//	return featureMatrixLoader.get();
	//}
private:

};

BOOST_FIXTURE_TEST_SUITE(SuperVectorCollectorTest, SuperVectorCollectorTestFixture)
BOOST_AUTO_TEST_CASE(SimplePca2Files)
{
	SuperVectors superVectors1;
	superVectors1.push_back((cv::Mat_<double>(2, 1) << 1, 0));
	SuperVectors superVectors2;
	superVectors2.push_back((cv::Mat_<double>(2, 1) << -1, 0));

	Mock<SuperVectorCalculator> superVectorCalculator;
	When(Method(superVectorCalculator, calculate)).Return(superVectors1);
	When(Method(superVectorCalculator, calculate)).Return(superVectors2);
	SuperVectorCalculator* superVectorCalculatorInstance = &superVectorCalculator.get();

	SuperVectorCollector superVectorCollector(*superVectorCalculatorInstance, "anyFile");

	Mock<MoodsInterface> moods;
	MoodsInterface* moodsInstance = &moods.get();

	Mock<InputFileNames> inputFileNames;
	When(Method(inputFileNames, fileNamesLeft)).Return(true);
	When(Method(inputFileNames, fileNamesLeft)).Return(false); // two files
	InputFileNames* inputFileNamesInstance = &inputFileNames.get();

	superVectorCollector.train(*moodsInstance, *inputFileNamesInstance);
}
BOOST_AUTO_TEST_SUITE_END()