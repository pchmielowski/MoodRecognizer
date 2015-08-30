#pragma once;
#include "SuperVectorCalculator.h"
#include "Types.h"
#include "SvmClassifier.h"
#include "PlotFilePreparator.h" // TODO forward it!
#include <vector>
#include <string>

class MoodsInterface;
class InputFileNames;
class Moods;
class PcaReductor;

class SuperVectorCollector {
public:
	SuperVectorCollector(SuperVectorCalculator& superVectorCalculator, PcaReductor& pcaReductor,
		SvmClassifier& svmClassifier);
	void prepareAndTrain(MoodsInterface& moods, InputFileNames& inputFileNames);



	void addAccuraciesToWriter(PlotFilePreparator& plotFilePreparator)
	{
		plotFilePreparator.addAccuracies(accuracies_);
	}

	Moods predictMoods(InputFileNames& inputFileNames);
private:
	std::string superVectorsFilePath = "C:\\OneDrive\\mgr\\superVectors.xml";
	SuperVectorCalculator* superVectorCalculator_ = nullptr;
	PcaReductor* pcaReductor_ = nullptr;
	SvmClassifier* svmClassifier_ = nullptr;
	std::vector<float> accuracies_;
	SuperVectors prepareSuperVectors(InputFileNames &inputFileNames);
	void trainPcaAndSvm(SuperVectors allSuperVectors, InputFileNames &inputFileNames, MoodsVector &moodsVector);

	SuperVectors loadSuperVectors();
};
