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
#include "PlotFilePreparator.h"

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
	bool isEq(const MoodsVector& a, const MoodsVector& b)
	{
		if (a.size() != b.size())
			return false;

		for (int superVectorIdx = 0; superVectorIdx < a.size(); ++superVectorIdx)
		{
			bool areDifferent = (a != b);
			if (areDifferent)
				return false;
		}
		return true;
	}
	const SuperVector for1File_1Alpha = (cv::Mat_<float>(2, 1) << 1, 4);
	const SuperVector for2File_1Alpha = (cv::Mat_<float>(2, 1) << 1.9, 2.2);
	const SuperVector for3File_1Alpha = (cv::Mat_<float>(2, 1) << 4, 2);
	const SuperVector for4File_1Alpha = (cv::Mat_<float>(2, 1) << 0.3, 2.7);

	const SuperVector reduced_for1File_1Alpha = (cv::Mat_<float>(1, 1) << 2);
	const SuperVector reduced_for2File_1Alpha = (cv::Mat_<float>(1, 1) << 1);
	const SuperVector reduced_for3File_1Alpha = (cv::Mat_<float>(1, 1) << 3);
	const SuperVector reduced_for4File_1Alpha = (cv::Mat_<float>(1, 1) << .8);

	const Mood moodFor1File = 0;
	const Mood moodFor2File = 0;
	const Mood moodFor3File = 3;
	const Mood moodFor4File = 2;

	const FileName fileName1 = "first.xml";
	const FileName fileName2 = "second.xml";
	const FileName fileName3 = "third.xml";
	const FileName fileName4 = "fourth.xml";
private:

};

BOOST_FIXTURE_TEST_SUITE(SuperVectorCollectorTest, SuperVectorCollectorTestFixture)
BOOST_AUTO_TEST_CASE(train_oneAlphaoneInputFile_correctCalls)
{
	// ARRANGE
	// superVectors
	const SuperVectors for1File = { for1File_1Alpha };
	const SuperVectors for1Alpha = { for1File_1Alpha };
	const SuperVectors reduced_for1Alpha = { reduced_for1File_1Alpha };
	// moods
	const MoodsVector moodsVector = { moodFor1File };

	// mocks
	Mock<SuperVectorCalculator> superVectorCalculator;
	When(Method(superVectorCalculator, calculate)).Return(for1File);
	SuperVectorCalculator& superVectorCalculatorInstance = superVectorCalculator.get();

	Mock<PcaReductor> pcaReductor;
	When(Method(pcaReductor, trainPca)).AlwaysReturn();
	When(Method(pcaReductor, reduce)).Return(reduced_for1File_1Alpha);
	PcaReductor& pcaReductorInstance = pcaReductor.get();

	Mock<SvmClassifier> svmClassifier;
	When(Method(svmClassifier, trainSvm)).AlwaysReturn();
	SvmClassifier& svmClassifierInstance = svmClassifier.get();
	
	SuperVectorCollector SUT(superVectorCalculatorInstance, pcaReductorInstance,
		svmClassifierInstance);

	Mock<MoodsInterface> moods;
	When(Method(moods, getNextMood)).Return(moodFor1File);
	MoodsInterface& moodsInstance = moods.get();

	Mock<InputFileNames> inputFileNames;
	When(Method(inputFileNames, fileNamesLeft)).
		Return(true)/*.Return(true)*/.Return(false).
		Return(true)/*.Return(true)*/.Return(false);
	When(Method(inputFileNames, getNextFileName)).Return("firstFileName.xml").AlwaysReturn();
	When(Method(inputFileNames, markAllAsUnread)).AlwaysReturn();
	InputFileNames& inputFileNamesInstance = inputFileNames.get();

	// ACT
	SUT.train(moodsInstance, inputFileNamesInstance);

	// ASSERT
	const int NUM_FILES = 1;
	const int NUM_ALPHAS = 1;
	Verify(Method(inputFileNames, getNextFileName)).Exactly(NUM_FILES + NUM_FILES*NUM_ALPHAS);
	Verify(Method(superVectorCalculator, calculate).Using("firstFileName.xml")).Exactly(1);
	Verify(Method(moods, getNextMood)).Exactly(NUM_FILES);

	Verify(Method(pcaReductor, trainPca).
		Matching([&](SuperVectors a){return isEq(a, for1Alpha); })).Exactly(1);
	Verify(Method(inputFileNames, markAllAsUnread)).Exactly(NUM_ALPHAS);
	Verify(Method(inputFileNames, fileNamesLeft)).Exactly(NUM_FILES + 1 + NUM_ALPHAS*(NUM_FILES + 1));
	Verify(Method(pcaReductor, reduce).
		Matching([&](const SuperVector& a){return isEq(a, for1File_1Alpha); })).Exactly(1);
	Verify(Method(svmClassifier, trainSvm).
		Matching([&](MoodsVector mv, SuperVectors a){return isEq(a, reduced_for1Alpha) && isEq(mv, moodsVector); }));
}
BOOST_AUTO_TEST_CASE(train_oneAlphaFourInputFiles_correctCalls)
{
	// ARRANGE
	// superVectors
	const SuperVectors for1File = { for1File_1Alpha };
	const SuperVectors for2File = { for2File_1Alpha };
	const SuperVectors for3File = { for3File_1Alpha };
	const SuperVectors for4File = { for4File_1Alpha };
	const SuperVectors for1Alpha = {
		for1File_1Alpha,
		for2File_1Alpha,
		for3File_1Alpha,
		for4File_1Alpha };
	const SuperVectors reduced_for1Alpha = {
		reduced_for1File_1Alpha,
		reduced_for2File_1Alpha,
		reduced_for3File_1Alpha,
		reduced_for4File_1Alpha };
	// moods
	const MoodsVector moodsVector = { moodFor1File, moodFor2File, moodFor3File, moodFor4File };

	// mocks
	Mock<SuperVectorCalculator> superVectorCalculator;
	When(Method(superVectorCalculator, calculate)).Return(for1File, for2File, for3File, for4File);
	SuperVectorCalculator& superVectorCalculatorInstance = superVectorCalculator.get();

	Mock<PcaReductor> pcaReductor;
	When(Method(pcaReductor, trainPca)).AlwaysReturn();
	When(Method(pcaReductor, reduce)).Return(
		reduced_for1File_1Alpha,
		reduced_for2File_1Alpha,
		reduced_for3File_1Alpha,
		reduced_for4File_1Alpha);
	PcaReductor& pcaReductorInstance = pcaReductor.get();

	Mock<SvmClassifier> svmClassifier;
	When(Method(svmClassifier, trainSvm)).AlwaysReturn();
	SvmClassifier& svmClassifierInstance = svmClassifier.get();
	
	SuperVectorCollector SUT(superVectorCalculatorInstance, pcaReductorInstance,
		svmClassifierInstance);

	Mock<MoodsInterface> moods;
	When(Method(moods, getNextMood)).Return(moodFor1File, moodFor2File, moodFor3File, moodFor4File);
	MoodsInterface& moodsInstance = moods.get();

	Mock<InputFileNames> inputFileNames;
	When(Method(inputFileNames, fileNamesLeft)).
		Return(true, true, true, true, false).
		Return(true, true, true, true, false);

	When(Method(inputFileNames, getNextFileName)).Return(fileName1, fileName2,
		fileName3, fileName4).AlwaysReturn();
	When(Method(inputFileNames, markAllAsUnread)).AlwaysReturn();
	InputFileNames& inputFileNamesInstance = inputFileNames.get();

	// ACT
	SUT.train(moodsInstance, inputFileNamesInstance);

	// ASSERT
	const int NUM_FILES = 4;
	const int NUM_ALPHAS = 1;
	Verify(Method(inputFileNames, getNextFileName)).Exactly(NUM_FILES + NUM_FILES*NUM_ALPHAS);
	Verify(Method(superVectorCalculator, calculate).Using(fileName1)).Exactly(1);
	Verify(Method(superVectorCalculator, calculate).Using(fileName2)).Exactly(1);
	Verify(Method(superVectorCalculator, calculate).Using(fileName3)).Exactly(1);
	Verify(Method(superVectorCalculator, calculate).Using(fileName4)).Exactly(1);
	Verify(Method(moods, getNextMood)).Exactly(NUM_FILES);

	Verify(Method(pcaReductor, trainPca).
		Matching([&](SuperVectors a){return isEq(a, for1Alpha); })).Exactly(1);
	Verify(Method(inputFileNames, markAllAsUnread)).Exactly(NUM_ALPHAS);
	Verify(Method(inputFileNames, fileNamesLeft)).Exactly(NUM_FILES + 1 + NUM_ALPHAS*(NUM_FILES + 1));

	Verify(Method(pcaReductor, reduce).
		Matching([&](const SuperVector& a){return isEq(a, for1File_1Alpha); }),
		Method(pcaReductor, reduce).
		Matching([&](const SuperVector& a){return isEq(a, for2File_1Alpha); }),
		Method(pcaReductor, reduce).
		Matching([&](const SuperVector& a){return isEq(a, for3File_1Alpha); }),
		Method(pcaReductor, reduce).
		Matching([&](const SuperVector& a){return isEq(a, for4File_1Alpha); })).Exactly(1);

	Verify(Method(svmClassifier, trainSvm).
		Matching([&](MoodsVector mv, SuperVectors a){return isEq(a, reduced_for1Alpha) && isEq(mv, moodsVector); }));
}
BOOST_AUTO_TEST_SUITE_END()