#include <boost/test/unit_test.hpp>
#include "Moods.h"

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
BOOST_AUTO_TEST_SUITE_END()