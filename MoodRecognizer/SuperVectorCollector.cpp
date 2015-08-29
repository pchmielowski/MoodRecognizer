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
	SuperVectors allSuperVectors;
	MoodsVector moodsVector;
	int numFilesRead = 0;

	while (inputFileNames.fileNamesLeft())
	{
		++numFilesRead;
		FileName fileName = inputFileNames.getNextFileName();
		bool hasRightExtension = fileName.substr(fileName.find_last_of(".") + 1) == "xml";
		if (!hasRightExtension)
			throw std::runtime_error("File " + fileName + " does not have .xml extension!");
		std::cout << fileName << std::endl;

		assert(superVectorCalculator_ != nullptr);
		SuperVector superVectorForFile = superVectorCalculator_->calculate(fileName);
		allSuperVectors.push_back(superVectorForFile);
		assert(allSuperVectors.size() == numFilesRead);

		moodsVector.push_back(moods.getNextMood());
		assert(moodsVector.size() == numFilesRead);
	}

	assert(pcaReductor_ != nullptr);
	pcaReductor_->trainPca(allSuperVectors);

	SuperVectors reducedSuperVectors;
	int fileIdx = 0;
	inputFileNames.markAllAsUnread();
	while (inputFileNames.fileNamesLeft())
	{
		inputFileNames.getNextFileName();
		SuperVector& superVectorForFile = allSuperVectors[fileIdx++];
		assert(superVectorForFile.cols == 1);

		SuperVector reducedSuperVectorForFile;
		reducedSuperVectorForFile = pcaReductor_->reduce(superVectorForFile);
		//std::cout << "SuperVector reduced for file no " << i << endl;
		assert(reducedSuperVectorForFile.cols == 1);
		reducedSuperVectors.push_back(reducedSuperVectorForFile);
	}

	assert(svmClassifier_ != nullptr);
	assert(reducedSuperVectors.size() != 0);
	assert(moodsVector.size() != 0);
	float accuracy = svmClassifier_->trainSvm(moodsVector, reducedSuperVectors);
	cout << "Accuracy: " << accuracy << endl;
}