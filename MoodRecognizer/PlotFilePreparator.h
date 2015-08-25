#pragma once;
#include "FileWriter.h"
#include "Types.h"
#include <string>
#include <sstream>
using namespace std;

class PlotFilePreparator
{
public:
	PlotFilePreparator(FileWriter& fileWriter);
	virtual void addAccuracies(vector<float> accuracies);
	virtual void write(const FileName fileName);
private:
	FileWriter* fileWriter_ = nullptr;
};