#pragma warning(push, 0)
#include <boost/test/unit_test.hpp>
#include <string>
#pragma warning(pop)
#include "InputFileNames.h"
#include "Mp3InputFileNames.h"
using namespace std;

const string TEST_TREE_PATH = "C:\\OneDrive\\C_MoodRecognizer\\UnitTests\\testTree\\";

BOOST_AUTO_TEST_SUITE(InputFileNamesTest)
BOOST_AUTO_TEST_CASE(fileNamesLeft_emptyQueue_false) 
{
	Mp3InputFileNames inputFileNames(TEST_TREE_PATH + "empty");

	BOOST_CHECK_EQUAL(inputFileNames.fileNamesLeft(), false);
}
BOOST_AUTO_TEST_CASE(fileNamesLeft_nonEmptyQueue_true)
{
	Mp3InputFileNames inputFileNames(TEST_TREE_PATH);

	BOOST_CHECK_EQUAL(inputFileNames.fileNamesLeft(), true);
}
BOOST_AUTO_TEST_CASE(getNextFileName_feedWithFolderTree_getCorrectNames) {
	Mp3InputFileNames inputFileNames(TEST_TREE_PATH);

	//int counter = 0;

	while (inputFileNames.fileNamesLeft()) {
		//counter++;
		string fileName = inputFileNames.getNextFileName();
		string extension = fileName.substr(fileName.find_last_of(".") + 1);
		BOOST_CHECK_EQUAL(extension, "mp3");
	}
}
BOOST_AUTO_TEST_CASE(getNextFileName_feedWithFolderTree_getNumberOfFiles) {
	Mp3InputFileNames inputFileNames(TEST_TREE_PATH);

	int counter = 0;

	while (inputFileNames.fileNamesLeft()) {
		inputFileNames.getNextFileName();
		counter++;
	}

	BOOST_CHECK_EQUAL(counter, 10);
}
BOOST_AUTO_TEST_CASE(getNextFileName_emptyQueue_throwsException) {
	Mp3InputFileNames inputFileNames(TEST_TREE_PATH + "empty");

	BOOST_REQUIRE_THROW(inputFileNames.getNextFileName(), std::runtime_error);

}
BOOST_AUTO_TEST_CASE(getNextFileName_wrongPath_throwsException) {
	BOOST_REQUIRE_THROW(Mp3InputFileNames inputFileNames("some wrong path"); , std::runtime_error);

}
BOOST_AUTO_TEST_SUITE_END()