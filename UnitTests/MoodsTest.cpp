#pragma warning(push, 0)
#include <boost/test/unit_test.hpp>
#include <fakeit.hpp>
#include <string>
#pragma warning(pop)
#include "Moods.h"
#include "FileReaderInterface.h"
using namespace fakeit;
using namespace std;

BOOST_AUTO_TEST_SUITE(MoodsTest)
BOOST_AUTO_TEST_CASE(getNextMood_addedMood3_return3) {
	Moods moods;

	moods.addMood(3);

	BOOST_CHECK_EQUAL(moods.getNextMood(), 3);
}
BOOST_AUTO_TEST_CASE(getNextMood_added1then2then3_return1then2then3) {
	Moods moods;

	moods.addMood(1);
	moods.addMood(2);
	moods.addMood(3);

	BOOST_CHECK_EQUAL(moods.getNextMood(), 1);
	BOOST_CHECK_EQUAL(moods.getNextMood(), 2);
	BOOST_CHECK_EQUAL(moods.getNextMood(), 3);
}
BOOST_AUTO_TEST_CASE(getNextMood_nothingAdded_throwException) {
	Moods moods;

	BOOST_REQUIRE_THROW(moods.getNextMood(), std::runtime_error);
}
BOOST_AUTO_TEST_CASE(getNextMood_loadedFileWith4Moods_gettedInRightOrder) {
	string fileContent = "1\n2\n3\n4\n";

	Mock<FileReaderInterface> mockFileReader;
	When(Method(mockFileReader, open)).Return();
	When(Method(mockFileReader, getContent)).Return(fileContent);
	FileReaderInterface& fileReader = mockFileReader.get();

	Moods moods(fileReader, "anyName");

	BOOST_CHECK_EQUAL(moods.getNextMood(), 1);
	BOOST_CHECK_EQUAL(moods.getNextMood(), 2);
	BOOST_CHECK_EQUAL(moods.getNextMood(), 3);
	BOOST_CHECK_EQUAL(moods.getNextMood(), 4);
}
BOOST_AUTO_TEST_CASE(getNextMood_loadedFileWith4MoodsInDifferentOrder_gettedInRightOrder) {
	string fileContent = "1\n4\n4\n1\n";

	Mock<FileReaderInterface> mockFileReader;
	When(Method(mockFileReader, open)).Return();
	When(Method(mockFileReader, getContent)).Return(fileContent);
	FileReaderInterface& fileReader = mockFileReader.get();

	Moods moods(fileReader, "anyName");

	BOOST_CHECK_EQUAL(moods.getNextMood(), 1);
	BOOST_CHECK_EQUAL(moods.getNextMood(), 4);
	BOOST_CHECK_EQUAL(moods.getNextMood(), 4);
	BOOST_CHECK_EQUAL(moods.getNextMood(), 1);
}
BOOST_AUTO_TEST_CASE(constructorWithFileReader_objectCreated_openCalledWithRighFileName) {
	string fileContent = "1\n4\n4\n1\n";

	Mock<FileReaderInterface> mockFileReader;
	When(Method(mockFileReader, open)).Return();
	When(Method(mockFileReader, getContent)).Return(fileContent);
	FileReaderInterface& fileReader = mockFileReader.get();

	Moods moods(fileReader, "fileName");

	Verify(Method(mockFileReader, open).Using("fileName")).AtLeastOnce();
}
BOOST_AUTO_TEST_SUITE_END()