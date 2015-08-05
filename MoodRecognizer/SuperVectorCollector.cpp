#include "SuperVectorCollector.h"
#include "Moods.h"
#include "Mp3InputFileNames.h"
#include "MatInputFileNames.h"


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
		allSuperVectors.insert(allSuperVectors.end(), superVectorsForFile.begin(), superVectorsForFile.end());
		++numFilesRead;
		int numSuperVectorsForFile = superVectorsForFile.size();
		assert(allSuperVectors.size() == numFilesRead*numSuperVectorsForFile);
		// TODO: get Mood
	}


}

SuperVectorCollector::SuperVectorCollector(SuperVectorCalculator& superVectorCalculator, 
	SvmClassifier& svmClassifier)
{

}
