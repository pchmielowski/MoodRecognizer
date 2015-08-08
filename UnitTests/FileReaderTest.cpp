#pragma warning(push, 0)
#include <boost/test/unit_test.hpp>
#include "FileReader.h"
#include <string>
#pragma warning(pop)
using namespace std;

BOOST_AUTO_TEST_SUITE(FileReadeTest)
BOOST_AUTO_TEST_CASE(getContent_realFile_goodContent) {
	FileReader SUT;

	SUT.open("C:\\OneDrive\\C_MoodRecognizer\\MoodRecognizer\\Debug\\FileToTestFileReader.txt");

	string content = SUT.getContent();

	BOOST_CHECK_EQUAL(content, "Some Content\nSecond Line");

}
BOOST_AUTO_TEST_SUITE_END()
