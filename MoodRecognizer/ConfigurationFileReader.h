#pragma once;
#include "Types.h"
#include <string>

class ConfigurationFileReader {
public:
	ConfigurationFileReader() {}
	virtual ~ConfigurationFileReader() {};
	virtual void open(FileName fileName) = 0;
	virtual std::string getContent() = 0;
};