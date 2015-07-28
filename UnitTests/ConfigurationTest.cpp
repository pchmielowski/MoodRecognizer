#define BOOST_TEST_MODULE Test
#include "boost/test/unit_test.hpp"
#include <fakeit.hpp>
// My classes:
#include "Configuration.h"
#include "Types.h"
#include "FileReaderInterface.h"
#include <string>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;
using namespace fakeit;

class ConfigurationTest {
public:
	Mock<FileReaderInterface> mockConfigurationFileReader;
	FileReaderInterface& mockFactory(string configurationFileContent)
	{
		When(Method(mockConfigurationFileReader, open)).AlwaysReturn();
		When(Method(mockConfigurationFileReader, getContent)).Return(configurationFileContent);
		return mockConfigurationFileReader.get();
	}
};

BOOST_AUTO_TEST_SUITE(ConfigurationTestSuite)
BOOST_FIXTURE_TEST_CASE(parseInputArguments_fullList_parseCorrectlyFilenames, ConfigurationTest) {
	char* argv[] = {
		"Mood.exe",
		"-t",
		"-u", "ubmFileName",
		"-i", "inputFilesPath",
		"-s", "svmFileName",
		"-p", "plotFileName",
		"-b", "baseFileName",
		"-m", "moodsFileName",
		"-c", "configurationFileName"
	};
	int argc = sizeof(argv) / sizeof(argv[0]);
	Configuration cfg;

	cfg.parseInputArguments(argc, argv);

	BOOST_CHECK(std::string("ubmFileName") == cfg.getUbmFileName());
	BOOST_CHECK(std::string("inputFilesPath") == cfg.getPathOfInputFiles());
	BOOST_CHECK(std::string("svmFileName") == cfg.getSvmModelFileName());
	BOOST_CHECK(std::string("plotFileName") == cfg.getPlotFileName());
	BOOST_CHECK(std::string("baseFileName") == cfg.getBaseFileName());
	BOOST_CHECK(std::string("moodsFileName") == cfg.getMoodsFileName());
	BOOST_CHECK(std::string("configurationFileName") == cfg.getConfigurationFileName());
}
BOOST_FIXTURE_TEST_CASE(parseInputArguments_fullList_parseCorrectlyMode, ConfigurationTest) {
	{
		char* argv[] = {
			"Mood.exe",
			"-t",
			"-u", "ubmFileName",
			"-i", "inputFilesPath",
			"-s", "svmFileName",
			"-p", "plotFileName",
			"-b", "baseFileName",
			"-m", "moodsFileName",
			"-c", "configurationFileName"
		};
		int argc = sizeof(argv) / sizeof(argv[0]);
		Configuration cfg;

		cfg.parseInputArguments(argc, argv);

		BOOST_CHECK(TRAIN_SVM == cfg.getMode());
	}
	{
	char* argv[] = {
		"Mood.exe",
		"-u", "ubmFileName",
		"-i", "inputFilesPath",
		"-s", "svmFileName",
		"-p", "plotFileName",
		"-b", "baseFileName",
		"-m", "moodsFileName",
		"-c", "configurationFileName"
	};
	int argc = sizeof(argv) / sizeof(argv[0]);
	Configuration cfg;

	cfg.parseInputArguments(argc, argv);

	BOOST_CHECK(MAKE_BASE == cfg.getMode());
}
}
BOOST_FIXTURE_TEST_CASE(parseInputArguments_xSwitch_throwsRuntimeErrorException, ConfigurationTest) {

	char* argv[] = {
		"Mood.exe",
		"-x", "someText"
	};
	int argc = sizeof(argv) / sizeof(argv[0]);
	Configuration cfg;

	BOOST_REQUIRE_THROW(cfg.parseInputArguments(argc, argv), std::runtime_error);
}
BOOST_FIXTURE_TEST_CASE(parseInputArguments_noSvmFile_throwsRuntimeErrorException, ConfigurationTest) {

	char* argv[] = {
		"Mood.exe",
		"-u", "ubmFileName",
		"-i", "inputFilesPath", // there is no SVM file
		"-p", "plotFileName",
		"-b", "baseFileName",
		"-m", "moodsFileName"
	};
	int argc = sizeof(argv) / sizeof(argv[0]);

	Configuration cfg;

	BOOST_REQUIRE_THROW(cfg.parseInputArguments(argc, argv), std::runtime_error);
}
BOOST_FIXTURE_TEST_CASE(parseConfigurationFile_RequiredToCallOpen_CallOpen, ConfigurationTest) {
	std::string configurationFileContent = "<alpha range=\"false\">\n";
	configurationFileContent += "<value>" + std::to_string(.6) + "</value>\n";
	configurationFileContent += "</alpha>\n";

	FileReaderInterface& mockConfigurationFileReaderInstance = mockFactory(configurationFileContent);

	Configuration cfg(mockConfigurationFileReaderInstance);
	
	cfg.parseConfigurationFile("anyfile");

	Verify(Method(mockConfigurationFileReader, open)).AtLeastOnce();
}
BOOST_FIXTURE_TEST_CASE(parseConfigurationFile_alphaEq_6_alphaEq_6, ConfigurationTest) {
	std::string configurationFileContent = "<alpha range=\"false\">\n";
	configurationFileContent += "<value>" + std::to_string(.6) + "</value>\n";
	configurationFileContent += "</alpha>\n";

	FileReaderInterface& mockConfigurationFileReaderInstance = mockFactory(configurationFileContent);
	Configuration cfg(mockConfigurationFileReaderInstance);

	cfg.parseConfigurationFile("anyfile");

	BOOST_CHECK_CLOSE(cfg.getAlpha(), .6, 0.1);
}
BOOST_FIXTURE_TEST_CASE(parseConfigurationFile_alphaEq0_alphaEq0, ConfigurationTest) {
	std::string configurationFileContent = "<alpha range=\"false\">\n";
	configurationFileContent += "<value>" + std::to_string(0) + "</value>\n";
	configurationFileContent += "</alpha>\n";

	Mock<FileReaderInterface> mockConfigurationFileReader;
	When(Method(mockConfigurationFileReader, open)).AlwaysReturn();
	When(Method(mockConfigurationFileReader, getContent)).Return(configurationFileContent);
	FileReaderInterface& mockConfigurationFileReaderInstance = mockConfigurationFileReader.get();
	Configuration cfg(mockConfigurationFileReaderInstance);

	cfg.parseConfigurationFile("anyfile");

	BOOST_CHECK_CLOSE(cfg.getAlpha(), 0, 0.1);
}
BOOST_FIXTURE_TEST_CASE(parseConfigurationFile_numberOfComponentsEq64_numComponentsEq64, ConfigurationTest) {
	std::string configurationFileContent = "<numberOfComponents range=\"false\">\n";
	configurationFileContent += "<value>" + std::to_string(64) + "</value>\n";
	configurationFileContent += "</numberOfComponents>\n";

	FileReaderInterface& mockConfigurationFileReaderInstance = mockFactory(configurationFileContent);
	Configuration cfg(mockConfigurationFileReaderInstance);

	cfg.parseConfigurationFile("anyfile");

	BOOST_CHECK_EQUAL(cfg.getNumComponents(), 64);
}
BOOST_FIXTURE_TEST_CASE(parseConfigurationFile_alphaAsRange_getRange, ConfigurationTest) {}
BOOST_FIXTURE_TEST_CASE(parseConfigurationFile_getValueWhenRangeIsPresent, ConfigurationTest) {

	//configurationFileContent = "<alpha range=false>\n";
	//configurationFileContent += "<min>" + std::to_string(1) + "</min>\n";
	//configurationFileContent += "<max>" + std::to_string(3) + "</max>\n";
	//configurationFileContent += "<value>" + std::to_string(alpha) + "</value>\n";
	//configurationFileContent += "</alpha>\n";
	//configurationFileContent += "<componentNumber range=false>\n";
	//configurationFileContent += "<min>" + std::to_string(1) + "</min>\n";
	//configurationFileContent += "<max>" + std::to_string(3) + "</max>\n";
	//configurationFileContent += "</componentNumber>";
}
BOOST_FIXTURE_TEST_CASE(parseConfigurationFile_wrongCfgFileException, ConfigurationTest) {  }
BOOST_AUTO_TEST_SUITE_END()
