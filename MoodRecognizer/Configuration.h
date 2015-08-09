#pragma once
#include <string>
#include <exception>
#include <iostream>
#include "Types.h"
#include <memory>
#include <vector>
using namespace std;

enum Mode {
	MAKE_BASE,
	TRAIN_SVM
};

class FileReader;

class Configuration {
public:
	Configuration(FileReader& fileReader);
	Configuration();
	~Configuration();
	Configuration(const Configuration& other);
	Configuration operator=(const Configuration& other);

	void parseInputArguments(const int argc, char* const argv[]);
	void parseConfigurationFile(const FileName fileName);

	FileName getPathOfInputFiles() const;
	FileName getSvmModelFileName() const;
	FileName getBaseFileName() const;
	FileName getUbmFileName() const;
	FileName getMoodsFileName() const;
	FileName getPlotFileName() const;
	FileName getConfigurationFileName() const;
	vector<int> getNumComponents() const;
	AlphaVector getAlpha() const;
	Mode getMode() const;
	bool shouldSavePlotFile() const;
	bool parsedOk();
private:
	FileReader* configurationFileReader_ = nullptr;
	Mode mode_;
	FileName folderOfFtMatrixesPath_;
	FileName svmModelFileName_;
	FileName baseFileName_;
	FileName configurationFileName_;
	FileName ubmFileName_;
	FileName moodsFileName_;
	FileName plotFileName_;
	vector<int> numOfComponents_;
	AlphaVector alpha_;

	bool savePlotFile_;
	bool saveSvmFile_;
	bool parsedOk_ = false;
};

