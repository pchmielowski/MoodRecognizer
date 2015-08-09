#pragma warning(push, 0)
#include <boost/test/unit_test.hpp>
//#include <fakeit.hpp>
//#include "opencv/cv.h"
//#include <boost/math/distributions/normal.hpp>
//#include <vector>
#pragma warning(pop)

#include "UbmLoader.h"

using namespace std;
//using namespace cv;

BOOST_AUTO_TEST_SUITE(UbmLoaderTest)
BOOST_AUTO_TEST_CASE(getUbm_realFile_weights) {

	UbmLoader SUT("integration_tests\\ubm.xml");

	Ubm result = SUT.getUbm();

	// to do: dodaæ kolumnê
	BOOST_CHECK_CLOSE(result.weights_.at<float>(0, 0), 2.5869811395596248e-001, .1);
	BOOST_CHECK_CLOSE(result.weights_.at<float>(0, 1), 2.9371393924854705e-001, .1);
	BOOST_CHECK_CLOSE(result.weights_.at<float>(0, 2), 1.6237960342979885e-001, .1);
	BOOST_CHECK_CLOSE(result.weights_.at<float>(0, 3), 5.0872897292405697e-002, .1);
	BOOST_CHECK_CLOSE(result.weights_.at<float>(0, 4), 2.3433544607326035e-001, .1);
}
BOOST_AUTO_TEST_CASE(getUbm_realFile_means) {

	UbmLoader SUT("integration_tests\\ubm.xml");

	Ubm result = SUT.getUbm();

	BOOST_CHECK_CLOSE(result.means_.at<float>(0, 0), 7.9047755331693452e+001, .1);
	BOOST_CHECK_CLOSE(result.means_.at<float>(1, 0), -3.3565650820110542e+000, .1);
	BOOST_CHECK_CLOSE(result.means_.at<float>(19, 4), 9.2612205876496295e-002, .1);
}
BOOST_AUTO_TEST_CASE(getUbm_realFile_covs) {

	UbmLoader SUT("integration_tests\\ubm.xml");

	Ubm result = SUT.getUbm();

	BOOST_CHECK_CLOSE(result.covs_[0].at<float>(0, 0), 2.1912278004379441e+001, .1);
	BOOST_CHECK_CLOSE(result.covs_[0].at<float>(1, 0), 4.5734249464081955e+000, .1);
	BOOST_CHECK_CLOSE(result.covs_[1].at<float>(0, 0), 7.0008852493511171e+001, .1);
	BOOST_CHECK_CLOSE(result.covs_[1].at<float>(1, 0), 1.3746214653180351e+001, .1);
}

BOOST_AUTO_TEST_CASE(getUbm_realFile_numComponents) {

	UbmLoader SUT("integration_tests\\ubm.xml");

	Ubm result = SUT.getUbm();

	BOOST_CHECK_EQUAL(result.numGaussComponents_, 5);
}
BOOST_AUTO_TEST_SUITE_END()
