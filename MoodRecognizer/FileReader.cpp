#include "FileReader.h"



FileReader::FileReader()
{

}

FileReader::~FileReader()
{

}

void FileReader::open(const FileName fileName)
{
	try {
		myfile.open(fileName, ios_base::in);
	}
	catch (...)
	{
		throw runtime_error("Failed to open " + fileName);
	}
}

std::string FileReader::getContent()
{
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

void FileReader::addNewLineSymbol()
{
	content_ += "\n";
}
