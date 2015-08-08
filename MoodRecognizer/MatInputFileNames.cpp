#include "MatInputFileNames.h"



bool MatInputFileNames::hasRightExtension(directory_iterator itr)
{
	return extension(itr->path()) == ".mat";
}

MatInputFileNames::MatInputFileNames(FileName inputDataFolderPath)
{
	makeQueue(inputDataFolderPath);
}
