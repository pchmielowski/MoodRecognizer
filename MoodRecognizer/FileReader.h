#pragma once;
#include "Types.h"
#include <string>

class FileReader {
public:
	FileReader() {}
	virtual ~FileReader() {};
	virtual void open(FileName fileName) {}
	virtual std::string getContent() { return ""; }
};