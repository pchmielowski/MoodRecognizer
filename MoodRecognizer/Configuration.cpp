#include "Configuration.h"
#include "boost\program_options.hpp"
#include <stdexcept>
#include "FileReader.h"
// for parsing XML
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <sstream>

using namespace std;

Configuration::Configuration() {
}

Configuration::Configuration(FileReader& fileReader)
{
	configurationFileReader_ = &fileReader;
}

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
		("-t", "mode=train")
		("-u", po::value<FileName>(), "ubm file name")
		("-i", po::value<FileName>(), "input files path")
		("-s", po::value<FileName>(), "svm file name")
		("-p", po::value<FileName>(), "plot file name")
		("-b", po::value<FileName>(), "base file name")
		("-m", po::value<FileName>(), "moods file name")
		("-c", po::value<FileName>(), "configuration file name")
		;

	po::variables_map variableMap;
	try {
		po::store(po::parse_command_line(argc, argv, description), variableMap);
		po::notify(variableMap);

		if (argc == 1) {
			cout << description << endl;
			return;
		}

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

			parseConfigurationFile(configurationFileName_);

			parsedOk_ = true;
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
void Configuration::parseConfigurationFile(const FileName fileName) {
	if (configurationFileReader_) {
		configurationFileReader_->open(fileName);
		stringstream fileContent;
		fileContent << configurationFileReader_->getContent();
		boost::property_tree::ptree xmlTree;
		try {
			boost::property_tree::read_xml(fileContent, xmlTree);
		}
		catch (boost::property_tree::ptree_error e) {
			cout << e.what() << endl;
		}
		try {
			string alphaAsString = xmlTree.get<std::string>("alpha.value");
			alpha_.push_back(atof(alphaAsString.c_str())); // ma braæ vector
		}
		catch (boost::property_tree::ptree_error e) {
			cout << e.what() << endl;
		}
		try {
			string numComponentsAsString = xmlTree.get<std::string>("numberOfComponents.value");
			numOfComponents_.push_back(atoi(numComponentsAsString.c_str()));
		}
		catch (boost::property_tree::ptree_error e) {
			cout << e.what() << endl;
		}
	}
}

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

vector<int> Configuration::getNumComponents() const {
	return numOfComponents_;
}

AlphaVector Configuration::getAlpha() const
{
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

bool Configuration::parsedOk()
{
	return parsedOk_;
}
