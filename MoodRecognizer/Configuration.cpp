#include "Configuration.h"
#include "boost\program_options.hpp"
#include <stdexcept>
#include "ConfigurationFileReader.h"

using namespace std;

Configuration::Configuration() {}

//Configuration::Configuration(ConfigurationFileReader& fileReader)
//{
//	configurationFileReader_ = &fileReader;
//}

Configuration::Configuration(const Configuration& other)
{
}

Configuration::~Configuration() {
	//delete configurationFileReader_;
}

void Configuration::parseInputArguments(const int argc, char* const argv[]) {
	namespace po = boost::program_options;

	po::options_description description("Allowed options");
	description.add_options()
		("-t", "mode")
		("-u", po::value<FileName>(), "ubmFileName")
		("-i", po::value<FileName>(), "inputFilesPath")
		("-s", po::value<FileName>(), "svmFileName")
		("-p", po::value<FileName>(), "plotFileName")
		("-b", po::value<FileName>(), "baseFileName")
		("-m", po::value<FileName>(), "moodsFileName")
		("-c", po::value<FileName>(), "configuration")
		;

	po::variables_map variableMap;
	try {
		po::store(po::parse_command_line(argc, argv, description), variableMap);
		po::notify(variableMap);

		if (variableMap.count("-t") > 0)
			mode_ = TRAIN_SVM;
		else
			mode_ = MAKE_BASE;

		if (variableMap.count("-u") &&
			variableMap.count("-i") &&
			variableMap.count("-s") &&
			variableMap.count("-b") &&
			variableMap.count("-c") &&
			variableMap.count("-m") &&
			variableMap.count("-p"))
		{
			ubmFileName_ = variableMap["-u"].as<FileName>();
			folderOfFtMatrixesPath_ = variableMap["-i"].as<FileName>();
			svmModelFileName_ = variableMap["-s"].as<FileName>();
			baseFileName_ = variableMap["-b"].as<FileName>();
			configurationFileName_ = variableMap["-c"].as<FileName>();
			moodsFileName_ = variableMap["-m"].as<FileName>();
			plotFileName_ = variableMap["-p"].as<FileName>();

			// configurationFileReader_->open(configurationFileName_);
		}
		else
			throw runtime_error("Missing argument!");
	}
	catch (runtime_error) {
		throw runtime_error("Missing argument!");
	}
	catch (...) {
		throw runtime_error("Unknown switch!");
	}
}
void Configuration::parseConfigurationFile(const FileName fileName) {}

FileName Configuration::getPathOfInputFiles() const {
	return folderOfFtMatrixesPath_;
}

FileName Configuration::getSvmModelFileName() const {
	return svmModelFileName_;
}

FileName Configuration::getBaseFileName() const {
	return baseFileName_;
}

FileName Configuration::getUbmFileName() const {
	return ubmFileName_;
}

FileName Configuration::getMoodsFileName() const {
	return moodsFileName_;
}

Mode Configuration::getMode() const {
	return mode_;
}

int Configuration::getNumComponents() const {
	return numOfComponents_;
}

float Configuration::getAlpha() const {
	return alpha_;
}

FileName Configuration::getPlotFileName() const {
	return plotFileName_;
}

FileName Configuration::getConfigurationFileName() const {
	return configurationFileName_;
}

bool Configuration::shouldSavePlotFile() const {
	return savePlotFile_;
}