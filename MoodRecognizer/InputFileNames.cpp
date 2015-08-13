#include "InputFileNames.h"
#include <iostream>
#include <algorithm>
using namespace std;

FileName InputFileNames::getNextFileName()
{
	try {
		FileName nextFileName = fileNameQueue_.front();
		fileNameQueue_.pop_front();
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
			fileNameQueue_.push_back(itr->path().string());
		}
	}

	std::sort(fileNameQueue_.begin(), fileNameQueue_.end(), InputFileNames::comparator);

	nonReducingFileNameQueue_ = fileNameQueue_;
}


bool InputFileNames::comparator(const string& left, const string& right)
{
	int leftNumber = extractNumber(left);
	int rightNumber = extractNumber(right);

	if (leftNumber < rightNumber)
		return true;
	else
		return false;
}

const bool InputFileNames::fileNamesLeft()
{
	return fileNameQueue_.empty() == 0;
}

void InputFileNames::markAllAsUnread()
{
	fileNameQueue_ = nonReducingFileNameQueue_;
}

