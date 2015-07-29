#pragma once;
#include "Types.h"
#include "InputFileNames.h"

using namespace std;

class Mp3InputFileNames : public InputFileNames{
public:
	Mp3InputFileNames(FileName inputDataFolderPath);
private:
	bool hasRightExtension(directory_iterator itr) override;
};
