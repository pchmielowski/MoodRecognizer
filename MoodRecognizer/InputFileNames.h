#pragma once;
#include "Types.h"
#include <boost/filesystem.hpp>
#include <queue>
using namespace boost::filesystem;
using namespace std;

class InputFileNames {
public:
	InputFileNames() {};
	FileName getNextFileName();
	const bool fileNamesLeft();
protected:
	void makeQueue(const path inputFolderPath);
private:
	virtual bool hasRightExtension(directory_iterator itr) = 0;
	queue<FileName> fileNameQueue_;

};
