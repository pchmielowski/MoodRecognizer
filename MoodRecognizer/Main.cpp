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
#include "PlotFilePreparator.h"
#include "FileWriter.h"

int main(int argc, char* argv[]) {

	FileReader cfgFileReader;
	Configuration cfg(cfgFileReader);
	try
	{
		cfg.parseInputArguments(argc, argv);
		if (!cfg.parsedOk())
			return -1;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << endl;
		return -1;
	}

	FeatureMatrixLoader*  featureMatrixLoader = new MatFeatureMatrixLoader(false);

	UbmLoader ubm(cfg.getUbmFileName());
	SuperVectorCalculator superVectorCalculator(*featureMatrixLoader, ubm);

	PcaReductor pcaReductor(cfg.getNumComponents());
	SvmClassifier svmClassifier(cfg.getSvmModelFileName());

	SuperVectorCollector superVectorCollector(superVectorCalculator, pcaReductor, svmClassifier);

	InputFileNames* inputFileNames = new MatInputFileNames(cfg.getPathOfInputFiles());

	if (cfg.getMode() == TRAIN_SVM)
	{
		FileReader moodsFileReader;
		Moods moods(moodsFileReader, cfg.getMoodsFileName());
		try {
			superVectorCollector.prepareAndTrain(moods, *inputFileNames);
			FileWriter plotFileWriter;
			PlotFilePreparator plotFilePreparator(plotFileWriter);
			superVectorCollector.addAccuraciesToWriter(plotFilePreparator);
			//plotFilePreparator.write("alpha_and_accuracy_plot.txt");
		}
		catch (exception& e) {
			cout << e.what() << endl;
		}
	}

	delete featureMatrixLoader;
	delete inputFileNames;

	cout << "OK!" << endl;
	return 0;
}