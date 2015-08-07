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
using namespace cv;

class SuperVectorCollectorTestFixture
{
public:
protected:
	bool isEq(const SuperVector& a, const SuperVector& b)
	{
		return countNonZero(a != b) == 0;
	}
	bool isEq(const SuperVectors& a, const SuperVectors& b)
	{
		if (a.size() != b.size())
			return false;

		for (int superVectorIdx = 0; superVectorIdx < a.size(); ++superVectorIdx)
		{
			SuperVector differenceVector = (a[superVectorIdx] != b[superVectorIdx]);
			bool areDifferent = (countNonZero(differenceVector) != 0);
			if (areDifferent)
				return false;
		}
		return true;
	}
private:

};

BOOST_FIXTURE_TEST_SUITE(SuperVectorCollectorTest, SuperVectorCollectorTestFixture)
BOOST_AUTO_TEST_CASE(train_oneAlphaoneInputFile_correctCalls)
{
	// ARRANGE
	// superVectors
	const SuperVector forFirstFile_firstAlpha = (cv::Mat_<float>(2, 1) << 1, 2);
	const SuperVectors forFirstFile = { forFirstFile_firstAlpha };
	const SuperVectors forFirstAlpha = { forFirstFile_firstAlpha };
	const SuperVector reduced_forFirstFile_firstAlpha = (cv::Mat_<float>(1, 1) << 1);
	const SuperVectors reduced_forFirstAlpha = { reduced_forFirstFile_firstAlpha };

	// mocks
	Mock<SuperVectorCalculator> superVectorCalculator;
	When(Method(superVectorCalculator, calculate)).Return(forFirstFile);
	SuperVectorCalculator& superVectorCalculatorInstance = superVectorCalculator.get();

	Mock<PcaReductor> pcaReductor;
	When(Method(pcaReductor, trainPca)).AlwaysReturn();
	When(Method(pcaReductor, reduce)).Return(reduced_forFirstFile_firstAlpha);
	PcaReductor& pcaReductorInstance = pcaReductor.get();

	Mock<SvmClassifier> svmClassifier;
	When(Method(svmClassifier, trainSvm)).AlwaysReturn();
	SvmClassifier& svmClassifierInstance = svmClassifier.get();

	AlphaVector alphas = { .3f };

	SuperVectorCollector SUT(superVectorCalculatorInstance, pcaReductorInstance,
		svmClassifierInstance, alphas);


	const Mood moodForFirstFile = 0;
	Mock<MoodsInterface> moods;
	When(Method(moods, getNextMood)).Return(moodForFirstFile);
	MoodsInterface& moodsInstance = moods.get();

	Mock<InputFileNames> inputFileNames;
	When(Method(inputFileNames, fileNamesLeft)).
		Return(true)/*.Return(true)*/.Return(false).
		Return(true)/*.Return(true)*/.Return(false);
	When(Method(inputFileNames, getNextFileName)).Return("firstFileName.mat")/*.Return("anotherFileName.mat")*/;
	When(Method(inputFileNames, markAllAsUnread)).AlwaysReturn();
	InputFileNames& inputFileNamesInstance = inputFileNames.get();

	// ACT
	SUT.train(moodsInstance, inputFileNamesInstance);

	// ASSERT
	const int NUM_FILES = 1;
	Verify(Method(inputFileNames, getNextFileName)).Exactly(NUM_FILES);
	Verify(Method(superVectorCalculator, calculate).Using("firstFileName.mat")).Exactly(1);
	//Verify(Method(superVectorCalculator, calculate).Using("anotherFileName.mat")).Exactly(1);
	Verify(Method(moods, getNextMood)).Exactly(NUM_FILES);

	const int NUM_ALPHAS = 1;
	Verify(Method(pcaReductor, trainPca).
		Matching([&](SuperVectors a){return isEq(a, forFirstAlpha); })).Exactly(1);
	Verify(Method(inputFileNames, markAllAsUnread)).Exactly(NUM_ALPHAS);
	Verify(Method(inputFileNames, fileNamesLeft)).Exactly(NUM_FILES + 1 + NUM_ALPHAS*(NUM_FILES + 1));
	Verify(Method(pcaReductor, reduce).
		Matching([&](const SuperVector& a){return isEq(a, forFirstFile_firstAlpha); })).Exactly(1);
	Verify(Method(svmClassifier, trainSvm).
		Matching([&](MoodsVector mv, SuperVectors a){return isEq(a, reduced_forFirstAlpha); }));
}
BOOST_AUTO_TEST_SUITE_END()