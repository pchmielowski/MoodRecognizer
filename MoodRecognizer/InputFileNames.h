#pragma once;
#include "Types.h"
#include <boost/filesystem.hpp>
#include <deque>
using namespace boost::filesystem;
using namespace std;

class InputFileNames {
public:
	InputFileNames() {};
	virtual FileName getNextFileName();
	virtual const bool fileNamesLeft();
	virtual void markAllAsUnread();
protected:
	void makeQueue(const path inputFolderPath);
private:
	virtual bool hasRightExtension(directory_iterator itr) = 0;
	deque<FileName> fileNameQueue_;
	deque<FileName> nonReducingFileNameQueue_;
	static bool comparator(const string& left, const string& right);

	static int extractNumber(const string &fullPath)
	{
		int slashPosition = fullPath.find_last_of("\\");
		int dotPosition = fullPath.find(".");
		string onlyName = fullPath.substr(slashPosition + 1, dotPosition);
		return atoi(onlyName.c_str());
	}

};
