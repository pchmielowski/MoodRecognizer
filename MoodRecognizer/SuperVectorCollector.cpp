#include "SuperVectorCollector.h"
#include "Moods.h"
#include "Mp3InputFileNames.h"
#include "MatInputFileNames.h"
#include "PcaReductor.h"
#include <iostream>
#include <string>

SuperVectorCollector::SuperVectorCollector(SuperVectorCalculator& superVectorCalculator,
	PcaReductor& pcaReductor,
	SvmClassifier& svmClassifier)
{
	superVectorCalculator_ = &superVectorCalculator;
	pcaReductor_ = &pcaReductor;
	svmClassifier_ = &svmClassifier;
}

void SuperVectorCollector::prepareAndTrain(MoodsInterface& moods, InputFileNames& inputFileNames)
{
	bool hasToPrepareSuperVectorsAndMoods = true;
	if (hasToPrepareSuperVectorsAndMoods)
	{
		SuperVectors allSuperVectors = prepareSuperVectors(inputFileNames);
		assert(allSuperVectors.size() > 0);

		cv::FileStorage superVectorsFile;
		std::string superVectorsFilePath = "";
		superVectorsFile.open(superVectorsFilePath, cv::FileStorage::WRITE);

		int superVectorIdx = 0;
		for (auto superVector : allSuperVectors)
		{
			superVectorsFile << "superVector"+to_string(superVectorIdx++);
			superVectorsFile << superVector;
		}

		superVectorsFile.release();
	}

	bool hasToTrainSvm = true;
	if (hasToTrainSvm)
	{
		SuperVectors allSuperVectors;
		// load allSuperVectors

		MoodsVector moodsVector;
		inputFileNames.markAllAsUnread();
		while (inputFileNames.fileNamesLeft())
		{
			moodsVector.push_back(moods.getNextMood());
		}
		trainPcaAndSvm(allSuperVectors, inputFileNames, moodsVector);
	}
}

SuperVectors SuperVectorCollector::prepareSuperVectors(InputFileNames &inputFileNames)
{
	SuperVectors allSuperVectors;
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
	}

	return allSuperVectors;
}

void SuperVectorCollector::trainPcaAndSvm(SuperVectors allSuperVectors, InputFileNames &inputFileNames, MoodsVector &moodsVector)
{
	// train Pca
	assert(pcaReductor_ != nullptr);
	pcaReductor_->trainPca(allSuperVectors);

	SuperVectors reducedSuperVectors;
	// reduce superVectors
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

	// train SVM
	assert(svmClassifier_ != nullptr);
	assert(reducedSuperVectors.size() != 0);
	assert(moodsVector.size() != 0);
	float accuracy = svmClassifier_->trainSvm(moodsVector, reducedSuperVectors);
	cout << "Accuracy: " << accuracy << endl;
}

Moods SuperVectorCollector::predictMoods(InputFileNames& inputFileNames)
{
	throw logic_error("Method SuperVectorCollector::predictMoods not implemented.");
}