#include "Mp3InputFileNames.h"
#include <boost/filesystem.hpp>
using namespace boost::filesystem;

Mp3InputFileNames::Mp3InputFileNames(FileName inputDataFolderPath)
{
	makeQueue(inputDataFolderPath);
}

bool Mp3InputFileNames::hasRightExtension(directory_iterator itr)
{
	return extension(itr->path()) == ".mp3";
}
