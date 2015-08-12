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
	const SuperVector for1File_2Alpha = (cv::Mat_<float>(2, 1) << 0, 0);
	const SuperVector for2File_2Alpha = (cv::Mat_<float>(2, 1) << 9, -9);

	const SuperVector reduced_for1File_1Alpha = (cv::Mat_<float>(1, 1) << 2);
	const SuperVector reduced_for2File_1Alpha = (cv::Mat_<float>(1, 1) << 1);
	const SuperVector reduced_for3File_1Alpha = (cv::Mat_<float>(1, 1) << 3);
	const SuperVector reduced_for4File_1Alpha = (cv::Mat_<float>(1, 1) << .8);
	const SuperVector reduced_for1File_2Alpha = (cv::Mat_<float>(1, 1) << -2);
	const SuperVector reduced_for2File_2Alpha = (cv::Mat_<float>(1, 1) << 1000);

	const Mood moodFor1File = 0;
	const Mood moodFor2File = 0;
	const Mood moodFor3File = 3;
	const Mood moodFor4File = 2;

	const FileName fileName1 = "first.mat";
	const FileName fileName2 = "second.mat";
	const FileName fileName3 = "third.mat";
	const FileName fileName4 = "fourth.mat";
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

	AlphaVector alphas = { .3f };

	SuperVectorCollector SUT(superVectorCalculatorInstance, pcaReductorInstance,
		svmClassifierInstance);

	Mock<MoodsInterface> moods;
	When(Method(moods, getNextMood)).Return(moodFor1File);
	MoodsInterface& moodsInstance = moods.get();

	Mock<InputFileNames> inputFileNames;
	When(Method(inputFileNames, fileNamesLeft)).
		Return(true)/*.Return(true)*/.Return(false).
		Return(true)/*.Return(true)*/.Return(false);
	When(Method(inputFileNames, getNextFileName)).Return("firstFileName.mat").AlwaysReturn();
	When(Method(inputFileNames, markAllAsUnread)).AlwaysReturn();
	InputFileNames& inputFileNamesInstance = inputFileNames.get();

	// ACT
	SUT.train(moodsInstance, inputFileNamesInstance);

	// ASSERT
	const int NUM_FILES = 1;
	const int NUM_ALPHAS = 1;
	Verify(Method(inputFileNames, getNextFileName)).Exactly(NUM_FILES + NUM_FILES*NUM_ALPHAS);
	Verify(Method(superVectorCalculator, calculate).Using("firstFileName.mat")).Exactly(1);
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

	AlphaVector alphas = { .3f };

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
BOOST_AUTO_TEST_CASE(train_2Alphas2InputFiles_correctCalls)
{
	// ARRANGE
	// superVectors
	const SuperVectors for1File = { for1File_1Alpha, for1File_2Alpha };
	const SuperVectors for2File = { for2File_1Alpha, for2File_2Alpha };
	const SuperVectors for1Alpha = {
		for1File_1Alpha,
		for2File_1Alpha };
	const SuperVectors for2Alpha = {
		for1File_2Alpha,
		for2File_2Alpha };
	const SuperVectors reduced_for1Alpha = {
		reduced_for1File_1Alpha,
		reduced_for2File_1Alpha };
	const SuperVectors reduced_for2Alpha = {
		reduced_for1File_2Alpha,
		reduced_for2File_2Alpha };
	// moods
	const MoodsVector moodsVector = { moodFor1File, moodFor2File };

	// mocks
	Mock<SuperVectorCalculator> superVectorCalculator;
	When(Method(superVectorCalculator, calculate)).Return(for1File, for2File);
	SuperVectorCalculator& superVectorCalculatorInstance = superVectorCalculator.get();

	Mock<PcaReductor> pcaReductor;
	When(Method(pcaReductor, trainPca)).AlwaysReturn();
	When(Method(pcaReductor, reduce)).Return(
		reduced_for1File_1Alpha,
		reduced_for2File_1Alpha,
		reduced_for1File_2Alpha,
		reduced_for2File_2Alpha);
	PcaReductor& pcaReductorInstance = pcaReductor.get();

	Mock<SvmClassifier> svmClassifier;
	When(Method(svmClassifier, trainSvm)).AlwaysReturn();
	SvmClassifier& svmClassifierInstance = svmClassifier.get();

	AlphaVector alphas = { .0f, 1.f };

	SuperVectorCollector SUT(superVectorCalculatorInstance, pcaReductorInstance,
		svmClassifierInstance);

	Mock<MoodsInterface> moods;
	When(Method(moods, getNextMood)).Return(moodFor1File, moodFor2File);
	MoodsInterface& moodsInstance = moods.get();

	Mock<InputFileNames> inputFileNames;
	When(Method(inputFileNames, fileNamesLeft)).
		Return(true, true, false).
		Return(true, true, false). // first alpha
		Return(true, true, false); // second alpha

	When(Method(inputFileNames, getNextFileName)).Return(fileName1, fileName2).AlwaysReturn();
	When(Method(inputFileNames, markAllAsUnread)).AlwaysReturn();
	InputFileNames& inputFileNamesInstance = inputFileNames.get();

	// ACT
	SUT.train(moodsInstance, inputFileNamesInstance);

	// ASSERT
	const int NUM_FILES = 2;
	const int NUM_ALPHAS = 2;
	Verify(Method(inputFileNames, getNextFileName)).Exactly(NUM_FILES + NUM_FILES*NUM_ALPHAS);
	Verify(Method(superVectorCalculator, calculate).Using(fileName1)).Exactly(1);
	Verify(Method(superVectorCalculator, calculate).Using(fileName2)).Exactly(1);
	Verify(Method(moods, getNextMood)).Exactly(NUM_FILES);

	Verify(Method(pcaReductor, trainPca).
		Matching([&](SuperVectors a){return isEq(a, for1Alpha); }), 
		Method(pcaReductor, trainPca).
		Matching([&](SuperVectors a){return isEq(a, for2Alpha); }));
	Verify(Method(inputFileNames, markAllAsUnread)).Exactly(NUM_ALPHAS);
	Verify(Method(inputFileNames, fileNamesLeft)).Exactly(NUM_FILES + 1 + NUM_ALPHAS*(NUM_FILES + 1));

	Verify(Method(pcaReductor, reduce).
		Matching([&](const SuperVector& a){return isEq(a, for1File_1Alpha); }),
		Method(pcaReductor, reduce).
		Matching([&](const SuperVector& a){return isEq(a, for2File_1Alpha); }),
		Method(pcaReductor, reduce).
		Matching([&](const SuperVector& a){return isEq(a, for1File_2Alpha); }),
		Method(pcaReductor, reduce).
		Matching([&](const SuperVector& a){return isEq(a, for2File_2Alpha); }));

	Verify(Method(svmClassifier, trainSvm).
		Matching([&](MoodsVector mv, SuperVectors a){return isEq(a, reduced_for1Alpha) && isEq(mv, moodsVector); }),
		Method(svmClassifier, trainSvm).
		Matching([&](MoodsVector mv, SuperVectors a){return isEq(a, reduced_for2Alpha) && isEq(mv, moodsVector); }));
}
BOOST_AUTO_TEST_CASE(addAccuracyToWriter_oneAccuracy_correctAccuracy)
{
	// ARRANGE
	// mocks
	Mock<SuperVectorCalculator> superVectorCalculator;
	When(Method(superVectorCalculator, calculate)).Return({ for1File_1Alpha, for1File_2Alpha });
	SuperVectorCalculator& superVectorCalculatorInstance = superVectorCalculator.get();

	Mock<PcaReductor> pcaReductor;
	When(Method(pcaReductor, trainPca)).AlwaysReturn();	
	When(Method(pcaReductor, reduce)).Return(
		reduced_for1File_1Alpha).Return(
		reduced_for1File_2Alpha);
	PcaReductor& pcaReductorInstance = pcaReductor.get();

	Mock<SvmClassifier> svmClassifier;
	When(Method(svmClassifier, trainSvm)).Return(.4).Return(.1);
	SvmClassifier& svmClassifierInstance = svmClassifier.get();

	SuperVectorCollector SUT(superVectorCalculatorInstance, pcaReductorInstance,
		svmClassifierInstance);

	// ---------
	Mock<MoodsInterface> moods;
	When(Method(moods, getNextMood)).Return(1);
	MoodsInterface& moodsInstance = moods.get();

	Mock<InputFileNames> inputFileNames;
	When(Method(inputFileNames, fileNamesLeft)).Return(true).Return(false).Return(true).
		Return(false).Return(true).Return(false);
	When(Method(inputFileNames, getNextFileName)).AlwaysReturn("file.mat");
	When(Method(inputFileNames, markAllAsUnread)).AlwaysReturn();
	InputFileNames& inputFileNamesInstance = inputFileNames.get();
	
	Mock<PlotFilePreparator> plotFilePreparator;
	When(Method(plotFilePreparator, addAccuracies)).Return();
	PlotFilePreparator& plotFilePreparatorInstance = plotFilePreparator.get();
	
	// ACT
	SUT.train(moodsInstance, inputFileNamesInstance);
	SUT.addAccuraciesToWriter(plotFilePreparatorInstance);

	// ASSERT
	Verify(Method(plotFilePreparator, addAccuracies).Using({ .4f, .1f }));
}
BOOST_AUTO_TEST_SUITE_END()