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

MatInputFileNames::MatInputFileNames(FileName inputDataFolderPath)
{
	makeQueue(inputDataFolderPath);
}

bool MatInputFileNames::hasRightExtension(directory_iterator itr)
{
	return extension(itr->path()) == ".mat";
}
