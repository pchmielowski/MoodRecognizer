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
	std::vector<float> accuracies_;
	void prepareSuperVectorsAndMoods(InputFileNames &inputFileNames, SuperVectors &allSuperVectors, MoodsVector &moodsVector, MoodsInterface &moods)
	{
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
	}
};
