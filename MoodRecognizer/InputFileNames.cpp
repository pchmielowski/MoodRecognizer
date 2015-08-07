#include "InputFileNames.h"
#include <iostream>
using namespace std;

FileName InputFileNames::getNextFileName()
{
	try {
		FileName nextFileName = fileNameQueue_.front();
		fileNameQueue_.pop();
		return nextFileName;
	}
	catch (...) {
		throw std::runtime_error("Unable to get next file name!");
	}
}

void InputFileNames::makeQueue(const path inputFolderPath)
{
	if (!exists(inputFolderPath) || !is_directory(inputFolderPath))
	{
		throw std::runtime_error("Path is not a directory!");
		return;
	}

	for (directory_iterator itr(inputFolderPath); itr != directory_iterator(); ++itr) 
	{
		if (is_directory(itr->status()))
		{
			makeQueue(itr->path());
		}
		else if (is_regular_file(itr->status()) && hasRightExtension(itr))
		{
			fileNameQueue_.push(itr->path().string());
		}
	}

	nonReducingFileNameQueue_ = fileNameQueue_;
}


const bool InputFileNames::fileNamesLeft()
{
	return fileNameQueue_.empty() == 0;
}

void InputFileNames::markAllAsUnread()
{
	fileNameQueue_ = nonReducingFileNameQueue_;
}

