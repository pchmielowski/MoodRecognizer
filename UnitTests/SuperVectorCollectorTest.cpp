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
	// Arrange 
	Mock<SuperVectorCalculator> superVectorCalculator;
	SuperVectorCalculator& superVectorCalculatorInstance = superVectorCalculator.get();

	Mock<PcaReductor> pcaReductor;
	PcaReductor& pcaReductorInstance = pcaReductor.get();

	Mock<SvmClassifier> svmClassifier;
	SvmClassifier& svmClassifierInstance = svmClassifier.get();

	AlphaVector alphas = { .3f };

	SuperVectorCollector SUT(superVectorCalculatorInstance, pcaReductorInstance,
		svmClassifierInstance, alphas);


	const Mood fakeMood = 0;
	Mock<MoodsInterface> moods;
	When(Method(moods, getNextMood)).AlwaysReturn(fakeMood);
	MoodsInterface& moodsInstance = moods.get();

	Mock<InputFileNames> inputFileNames;
	When(Method(inputFileNames, fileNamesLeft)).Return(true).Return(false);
	InputFileNames& inputFileNamesInstance = inputFileNames.get();

	// Act
	SUT.train(moodsInstance, inputFileNamesInstance);

	// verify  (liczba plików) x call do:
	// inputFileNames_->getNextFileName();
	// superVectorCalculator_->calculate(fileName); <--- fileName otrzymany wy¿ej
	// moods.getNextMood();

	// (liczba alf) call do:
	// pcaReductor_->trainPca(superVectorsForAlpha);
	// inputFileNames_->reset();
	//    (liczba plików)  x
	//    while (inputFileNames_->fileNamesLeft())
	//    reducedSuperVectorForFileAndAlpha = pcaReductor_->reduce(superVectorForFileAndAlpha);

}
// dodaæ poprawne moody do Svm
BOOST_AUTO_TEST_SUITE_END()