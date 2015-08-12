#pragma once;
#include "Types.h"
#include <string>
#include <fstream>
using namespace std;

class FileReader 
{
public:
	FileReader();
	virtual ~FileReader();
	virtual void open(const FileName fileName);
	virtual std::string getContent();

private:
	string content_;
	ifstream myfile;
	void addNewLineSymbol();
};


