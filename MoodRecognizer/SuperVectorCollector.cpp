#include "SuperVectorCollector.h"
#include "Moods.h"
#include "Mp3InputFileNames.h"
#include "MatInputFileNames.h"
#include "PcaReductor.h"

SuperVectorCollector::SuperVectorCollector(SuperVectorCalculator& superVectorCalculator, 
	PcaReductor& pcaReductor,
	SvmClassifier& svmClassifier, AlphaVector alphas)
{
	superVectorCalculator_ = &superVectorCalculator;
	pcaReductor_ = &pcaReductor;
	svmClassifier_ = &svmClassifier;
	alphas_ = alphas;
}

Moods SuperVectorCollector::predictMoods(InputFileNames& inputFileNames)
{
	return Moods();
}

void SuperVectorCollector::train(MoodsInterface& moods, InputFileNames& inputFileNames)
{
	SuperVectors allSuperVectors;
	MoodsVector moodsVector;
	int numFilesRead = 0;

	while (inputFileNames.fileNamesLeft())
	{
		++numFilesRead;
		FileName fileName = inputFileNames.getNextFileName();
		bool hasRightExtension = fileName.substr(fileName.find_last_of(".") + 1) == "mat";
		if (!hasRightExtension)
			throw std::runtime_error("File " + fileName + " does not have .mat extension!");

		assert(superVectorCalculator_ != nullptr);
		SuperVectors superVectorsForFile = superVectorCalculator_->calculate(fileName);

		moodsVector.push_back(moods.getNextMood());
		assert(moodsVector.size() == numFilesRead);

		appendSuperVectorToAllSuperVectors(allSuperVectors, superVectorsForFile);

		int numSuperVectorsForFile = superVectorsForFile.size();
		assert(allSuperVectors.size() == numFilesRead*numSuperVectorsForFile);
		assert(numSuperVectorsForFile == alphas_.size());
	}

	assert(alphas_.size() > 0);
	for (auto alpha : alphas_)
	{
		SuperVectors superVectorsForAlpha = allSuperVectors;
		SuperVectors reducedSuperVectorsForAlpha;
		assert(pcaReductor_ != nullptr);
		pcaReductor_->trainPca(superVectorsForAlpha);

		inputFileNames.markAllAsUnread();
		while (inputFileNames.fileNamesLeft())
		{
			SuperVector& superVectorForFileAndAlpha = allSuperVectors[0];
			assert(superVectorForFileAndAlpha.cols == 1);

			SuperVector reducedSuperVectorForFileAndAlpha;
			reducedSuperVectorForFileAndAlpha = pcaReductor_->reduce(superVectorForFileAndAlpha);
			assert(reducedSuperVectorForFileAndAlpha.cols == 1);
			reducedSuperVectorsForAlpha.push_back(reducedSuperVectorForFileAndAlpha);
		}

		assert(svmClassifier_ != nullptr);
		assert(reducedSuperVectorsForAlpha.size() != 0);
		assert(moodsVector.size() != 0);
		svmClassifier_->trainSvm(moodsVector, reducedSuperVectorsForAlpha);
	}

}

void SuperVectorCollector::appendSuperVectorToAllSuperVectors(SuperVectors &allSuperVectors, SuperVectors &superVectorsForFile)
{
	int initialSize = allSuperVectors.size();
	if (initialSize == 0)
		allSuperVectors = superVectorsForFile;
	else
		allSuperVectors.insert(allSuperVectors.end(),
		superVectorsForFile.begin(), superVectorsForFile.end());

	assert(allSuperVectors.size() == initialSize + superVectorsForFile.size());
}
