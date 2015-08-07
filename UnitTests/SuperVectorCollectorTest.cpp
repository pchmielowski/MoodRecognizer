#pragma warning(push, 0)
#include <boost/test/unit_test.hpp>
#include <fakeit.hpp>
#include "opencv/cv.h"
#pragma warning(pop)

#include "SuperVectorCollector.h"
#include "MoodsInterface.h"
#include "InputFileNames.h"
#include "PcaReductor.h"
#include "SuperVectorCalculator.h"
#include "Types.h"

using namespace fakeit;
using namespace std;

class SuperVectorCollectorTestFixture
{
public:

private:

};

BOOST_FIXTURE_TEST_SUITE(SuperVectorCollectorTest, SuperVectorCollectorTestFixture)
BOOST_AUTO_TEST_CASE(train_oneAlphaTwoInputFiles_correctCalls)
{
	// ARRANGE
	SuperVector someSuperVector = (cv::Mat_<float>(2, 1) << 1, 2);
	SuperVectors fakeSuperVectors;
	fakeSuperVectors.push_back(someSuperVector);
	Mock<SuperVectorCalculator> superVectorCalculator;
	When(Method(superVectorCalculator, calculate)).AlwaysReturn(fakeSuperVectors);
	SuperVectorCalculator& superVectorCalculatorInstance = superVectorCalculator.get();

	SuperVector fakeReducedSuperVector = (cv::Mat_<float>(1, 1) << 1);
	Mock<PcaReductor> pcaReductor;
	When(Method(pcaReductor, trainPca)).AlwaysReturn();
	When(Method(pcaReductor, reduce)).AlwaysReturn(fakeReducedSuperVector);
	PcaReductor& pcaReductorInstance = pcaReductor.get();

	Mock<SvmClassifier> svmClassifier;
	When(Method(svmClassifier, trainSvm)).Return();
	SvmClassifier& svmClassifierInstance = svmClassifier.get();

	AlphaVector alphas = { .3f };

	SuperVectorCollector SUT(superVectorCalculatorInstance, pcaReductorInstance,
		svmClassifierInstance, alphas);


	const Mood fakeMood = 0;
	Mock<MoodsInterface> moods;
	When(Method(moods, getNextMood)).AlwaysReturn(fakeMood);
	MoodsInterface& moodsInstance = moods.get();

	Mock<InputFileNames> inputFileNames;
	When(Method(inputFileNames, fileNamesLeft)).
		Return(true).Return(true).Return(false).
		Return(true).Return(true).Return(false);
	When(Method(inputFileNames, getNextFileName)).Return("someFileName.mat").Return("anotherFileName.mat");
	When(Method(inputFileNames, markAllAsUnread)).AlwaysReturn();
	InputFileNames& inputFileNamesInstance = inputFileNames.get();

	// ACT
	SUT.train(moodsInstance, inputFileNamesInstance);

	// ASSERT
	const int NUM_FILES = 2;
	Verify(Method(inputFileNames, getNextFileName)).Exactly(NUM_FILES);
	Verify(Method(superVectorCalculator, calculate).Using("someFileName.mat")).Exactly(1);
	Verify(Method(superVectorCalculator, calculate).Using("anotherFileName.mat")).Exactly(1);
	Verify(Method(moods, getNextMood)).Exactly(NUM_FILES);
	
	const int NUM_ALPHAS = 1;
	Verify(Method(pcaReductor, trainPca)).Exactly(NUM_ALPHAS); // .Using(superVectorsForAlpha)
	Verify(Method(inputFileNames, markAllAsUnread)).Exactly(NUM_ALPHAS);
	Verify(Method(inputFileNames, fileNamesLeft)).Exactly(NUM_FILES+1+NUM_ALPHAS*(NUM_FILES+1));
	Verify(Method(pcaReductor, reduce)).Exactly(NUM_ALPHAS*NUM_FILES); // .Using(superVectorForFileAndAlpha)

}
// dodaæ poprawne moody do Svm
BOOST_AUTO_TEST_SUITE_END()