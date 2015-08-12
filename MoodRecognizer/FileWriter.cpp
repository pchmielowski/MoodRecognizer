#include "FileWriter.h"

using namespace std;


FileWriter::FileWriter()
{

}

FileWriter::~FileWriter()
{

}

void FileWriter::open(const FileName fileName)
{
	try {
		myfile.open(fileName);
	}
	catch (...)
	{
		throw runtime_error("Failed to open " + fileName);
	}
}

void FileWriter::appendContent(string content)
{
	content_ += content;
}

void FileWriter::saveAndClose() // TODO: byæ mo¿e niepotrzebna metoda
{
	myfile << content_;
	myfile.close();
}
