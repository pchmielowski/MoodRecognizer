#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(AnotherSuiteInAnotherFile)
BOOST_AUTO_TEST_CASE(AnyCase) {
	BOOST_CHECK_EQUAL(1, 1);
}

BOOST_AUTO_TEST_SUITE_END()