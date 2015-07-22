#pragma once;
#include "Types.h"

class InputFileNames {
	InputFileNames(FileName inputDataFolderPath);
	FileName getNextFileName();
};
