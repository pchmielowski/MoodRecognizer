#pragma once;
#include "Types.h"
#include <string>
#include <fstream>
using namespace std;

class FileReader {
public:
	FileReader() {}
	virtual ~FileReader() {};
	virtual void open(const FileName fileName) {
		try {
			myfile.open(fileName, ios_base::in);
		}
		catch (...)
		{
			throw runtime_error("Failed to open " + fileName);
		}
	}
	virtual std::string getContent() {
		if (!myfile.is_open())
		{
			throw runtime_error("File is not open!");
		}
		string line;
		bool isFirstLine = true;
		while (getline(myfile, line))
		{
			if (!isFirstLine) {
				addNewLineSymbol();
			}
			content_ += line;
			isFirstLine = false;
		}
		myfile.close();
		return content_;
	}

private:
	string content_;
	ifstream myfile;
	void addNewLineSymbol()
	{
		content_ += "\n";
	}
};


