#include "FileReader.h"
#include "Configuration.h"
#include "FeatureMatrixLoader.h"
#include "MatFeatureMatrixLoader.h"
#include "UbmLoader.h"
#include "SuperVectorCalculator.h"
#include "PcaReductor.h"
#include "SvmClassifier.h"
#include "SuperVectorCollector.h"
#include "InputFileNames.h"
#include "MatInputFileNames.h"
#include "Moods.h"


int main(int argc, char* argv[]) { 
	
	FileReader cfgFileReader;
	Configuration cfg(cfgFileReader);
	try
	{
		cfg.parseInputArguments(argc, argv);
		if (!cfg.parsedOk())
			return -1;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << endl;
		return -1;
	}

	FeatureMatrixLoader*  featureMatrixLoader = new MatFeatureMatrixLoader(false);

	UbmLoader ubm(cfg.getUbmFileName());
	SuperVectorCalculator superVectorCalculator(*featureMatrixLoader, ubm, cfg.getAlpha());

	PcaReductor pcaReductor(cfg.getNumComponents());
	SvmClassifier svmClassifier(cfg.getSvmModelFileName());

	SuperVectorCollector superVectorCollector(superVectorCalculator, pcaReductor, svmClassifier);
	
	InputFileNames* inputFileNames = new MatInputFileNames(cfg.getPathOfInputFiles());

	if (cfg.getMode() == TRAIN_SVM) 
	{
		FileReader moodsFileReader;
		Moods moods(moodsFileReader, cfg.getMoodsFileName());
		superVectorCollector.train(moods, *inputFileNames);
	}
	
	delete featureMatrixLoader;
	delete inputFileNames;

	return 0;  
}