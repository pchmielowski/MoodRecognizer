#include "SuperVectorCollector.h"
#include "Moods.h"
#include "Mp3InputFileNames.h"
#include "MatInputFileNames.h"
#include "PcaReductor.h"

Moods SuperVectorCollector::predictMoods(InputFileNames& inputFileNames)
{
	return Moods();
}

void SuperVectorCollector::train(MoodsInterface& moods, InputFileNames& inputFileNames)
{
	SuperVectors allSuperVectors;
	inputFileNames_ = &inputFileNames;
	int numFilesRead = 0;

	while (inputFileNames_->fileNamesLeft())
	{
		FileName fileName = inputFileNames_->getNextFileName();

		SuperVectors superVectorsForFile = superVectorCalculator_->calculate(fileName);

		moods.getNextMood();

		allSuperVectors.insert(allSuperVectors.end(), superVectorsForFile.begin(), superVectorsForFile.end());
		++numFilesRead;
		int numSuperVectorsForFile = superVectorsForFile.size();
		assert(allSuperVectors.size() == numFilesRead*numSuperVectorsForFile);
	}

	for (auto alpha : *alphas_)
	{
	SuperVectors superVectorsForAlpha;
		pcaReductor_->trainPca(superVectorsForAlpha);

		inputFileNames_->reset();
		while (inputFileNames_->fileNamesLeft())
		{
			SuperVector superVectorForFileAndAlpha;
			SuperVector reducedSuperVectorForFileAndAlpha;
			reducedSuperVectorForFileAndAlpha = pcaReductor_->reduce(superVectorForFileAndAlpha);
		}
		// trainSvm(moods, superVectors);
	}

}

SuperVectorCollector::SuperVectorCollector(SuperVectorCalculator& superVectorCalculator, PcaReductor& pcaReductor, 
	SvmClassifier& svmClassifier, const AlphaVector alphas)
{

}
