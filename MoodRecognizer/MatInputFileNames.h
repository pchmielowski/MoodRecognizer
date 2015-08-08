#pragma once;
#include "Types.h"
#include "InputFileNames.h"

using namespace std;

class MatInputFileNames : public InputFileNames{
public:
	MatInputFileNames(FileName inputDataFolderPath);
private:
	bool hasRightExtension(directory_iterator itr) override;
};
