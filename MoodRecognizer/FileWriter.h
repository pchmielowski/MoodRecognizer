#pragma once;
#include "Types.h"
#include <string>
using namespace std;

class FileWriter
{
public:
	FileWriter();
	virtual ~FileWriter();
	virtual void open(const FileName fileName);
	virtual void appendContent(string content);
	virtual void saveAndClose();
};