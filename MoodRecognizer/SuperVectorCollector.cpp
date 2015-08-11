#include "SuperVectorCollector.h"
#include "Moods.h"
#include "Mp3InputFileNames.h"
#include "MatInputFileNames.h"
#include "PcaReductor.h"
#include <iostream>

SuperVectorCollector::SuperVectorCollector(SuperVectorCalculator& superVectorCalculator, 
	PcaReductor& pcaReductor,
	SvmClassifier& svmClassifier)
{
	superVectorCalculator_ = &superVectorCalculator;
	pcaReductor_ = &pcaReductor;
	svmClassifier_ = &svmClassifier;
}

Moods SuperVectorCollector::predictMoods(InputFileNames& inputFileNames)
{
	return Moods();
}

void SuperVectorCollector::train(MoodsInterface& moods, InputFileNames& inputFileNames)
{
	vector<SuperVectors> allSuperVectors;
	MoodsVector moodsVector;
	int numFilesRead = 0;
	int numSuperVectorsForFile;

	while (inputFileNames.fileNamesLeft())
	{
		++numFilesRead;
		FileName fileName = inputFileNames.getNextFileName();
		bool hasRightExtension = fileName.substr(fileName.find_last_of(".") + 1) == "mat";
		if (!hasRightExtension)
			throw std::runtime_error("File " + fileName + " does not have .mat extension!");

		assert(superVectorCalculator_ != nullptr);
		SuperVectors superVectorsForFile = superVectorCalculator_->calculate(fileName);
		std::cout << "SuperVectorCalculated for file no " << numFilesRead << endl;

		moodsVector.push_back(moods.getNextMood());
		assert(moodsVector.size() == numFilesRead);

		appendSuperVectorToAllSuperVectors(allSuperVectors, superVectorsForFile);

		numSuperVectorsForFile = superVectorsForFile.size();
		assert(allSuperVectors.size() == numFilesRead);
	}

	
	for (int alphaIdx = 0; alphaIdx < numSuperVectorsForFile; ++alphaIdx)
	{
		SuperVectors superVectorsForAlpha;
		for (auto superVectorsForFile : allSuperVectors)
			superVectorsForAlpha.push_back(superVectorsForFile[alphaIdx]);

		SuperVectors reducedSuperVectorsForAlpha;
		assert(pcaReductor_ != nullptr);
		pcaReductor_->trainPca(superVectorsForAlpha);
		std::cout << "PCA trained" << endl;
		inputFileNames.markAllAsUnread();

		int fileIdx = 0;
		while (inputFileNames.fileNamesLeft())
		{
			inputFileNames.getNextFileName();
			SuperVector& superVectorForFileAndAlpha = superVectorsForAlpha[fileIdx++];
			assert(superVectorForFileAndAlpha.cols == 1);

			SuperVector reducedSuperVectorForFileAndAlpha;
			reducedSuperVectorForFileAndAlpha = pcaReductor_->reduce(superVectorForFileAndAlpha);
			//std::cout << "SuperVector reduced for file no " << i << endl;
			assert(reducedSuperVectorForFileAndAlpha.cols == 1);
			reducedSuperVectorsForAlpha.push_back(reducedSuperVectorForFileAndAlpha);
		}

		assert(svmClassifier_ != nullptr);
		assert(reducedSuperVectorsForAlpha.size() != 0);
		assert(moodsVector.size() != 0);
		svmClassifier_->trainSvm(moodsVector, reducedSuperVectorsForAlpha);
	}

}

void SuperVectorCollector::appendSuperVectorToAllSuperVectors(vector<SuperVectors> &allSuperVectors, 
	SuperVectors &superVectorsForFile)
{
	int initialSize = allSuperVectors.size();
	allSuperVectors.push_back(superVectorsForFile);
	assert(allSuperVectors.size() == initialSize + 1);
}
