#pragma once
#include <string>
#include <exception>
#include <iostream>
#include "Types.h"

enum Mode {
	MAKE_BASE,
	TRAIN_SVM
};

class Configuration {
public:
	Configuration();
	~Configuration();

	void parseConfigurationFile(const FileName fileName);
	void parseInputArguments(const int argc, char* const argv[]);

	FileName getPathOfInputFiles() const;
	FileName getSvmModelFileName() const;
	FileName getBaseFileName() const;
	FileName getUbmFileName() const;
	FileName getMoodsFileName() const;
	FileName getPlotFileName() const;
	FileName getConfigurationFileName() const;
	int getNumComponents() const;
	float getAlpha() const;
	Mode getMode() const;
	bool shouldSavePlotFile() const;
private:
	Mode mode_;
	FileName folderOfFtMatrixesPath_;
	FileName svmModelFileName_;
	FileName baseFileName_;
	FileName configurationFileName_;
	FileName ubmFileName_;
	FileName moodsFileName_;
	FileName plotFileName_;
	int numOfComponents_;
	float alpha_;

	bool savePlotFile_;
	bool saveSvmFile_;

};

