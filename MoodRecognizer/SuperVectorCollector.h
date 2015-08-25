#pragma once;
#include "SuperVectorCalculator.h"
#include "Types.h"
#include "SvmClassifier.h"
#include "PlotFilePreparator.h" // TODO forward it!
#include <vector>

class MoodsInterface;
class InputFileNames;
class Moods;
class PcaReductor;

class SuperVectorCollector {
public:
	SuperVectorCollector(SuperVectorCalculator& superVectorCalculator, PcaReductor& pcaReductor,
		SvmClassifier& svmClassifier);
	void train(MoodsInterface& moods, InputFileNames& inputFileNames);

	void addAccuraciesToWriter(PlotFilePreparator& plotFilePreparator)
	{
		plotFilePreparator.addAccuracies(accuracies_);
	}

	Moods predictMoods(InputFileNames& inputFileNames);
private:
	SuperVectorCalculator* superVectorCalculator_ = nullptr;
	PcaReductor* pcaReductor_ = nullptr;
	SvmClassifier* svmClassifier_ = nullptr;
	void appendSuperVectorToAllSuperVectors(SuperVectors &allSuperVectors, SuperVector &superVectorsForFile);
	std::vector<float> accuracies_;
};
