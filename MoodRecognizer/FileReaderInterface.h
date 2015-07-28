#pragma once;
#include "Types.h"
#include <string>

class FileReaderInterface {
public:
	FileReaderInterface() {}
	virtual ~FileReaderInterface() {};
	virtual void open(FileName fileName) = 0;
	virtual std::string getContent() = 0;
};