#define BOOST_TEST_MODULE Test
#pragma warning(push, 0)
#include "boost/test/unit_test.hpp"
#include <fakeit.hpp>
#pragma warning(pop)
// My classes:
#include "Configuration.h"
#include "Types.h"
#include "FileReader.h"
#include <string>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;
using namespace fakeit;

class ConfigurationTest {
public:
	Mock<FileReader> mockConfigurationFileReader;
	FileReader& mockFactory(string configurationFileContent)
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

	FileReader& mockConfigurationFileReaderInstance = mockFactory(configurationFileContent);

	Configuration cfg(mockConfigurationFileReaderInstance);
	
	cfg.parseConfigurationFile("anyfile");

	Verify(Method(mockConfigurationFileReader, open)).AtLeastOnce();
}
BOOST_FIXTURE_TEST_CASE(parseConfigurationFile_numberOfComponentsEq64_numComponentsEq64, ConfigurationTest) {
	std::string configurationFileContent = "<numberOfComponents range=\"false\">\n";
	configurationFileContent += "<value>" + std::to_string(64) + "</value>\n";
	configurationFileContent += "</numberOfComponents>\n";

	FileReader& mockConfigurationFileReaderInstance = mockFactory(configurationFileContent);
	Configuration cfg(mockConfigurationFileReaderInstance);

	cfg.parseConfigurationFile("anyfile");

	BOOST_CHECK_EQUAL(cfg.getNumComponents()[0], 64);
}
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
BOOST_FIXTURE_TEST_CASE(parsedOk_listForTraining_true, ConfigurationTest) {
	char* argv[] = {
		"Mood.exe",
		"-i", "inputFilesPath",
		"-s", "svmFileName",
		"-b", "baseFileName",
		"-c", "configurationFileName"
	};
	int argc = sizeof(argv) / sizeof(argv[0]);
	Configuration cfg;

	cfg.parseInputArguments(argc, argv);

	bool result = cfg.parsedOk();

	BOOST_CHECK_EQUAL(result, true);
}
BOOST_FIXTURE_TEST_CASE(parsedOk_listForMakingBase_true, ConfigurationTest) {
	char* argv[] = {
		"Mood.exe",
		"-t",
		"-u", "ubmFileName",
		"-i", "inputFilesPath",
		"-s", "svmFileName",
		"-p", "plotFileName",
		"-m", "moodsFileName",
		"-c", "configurationFileName"
	};
	int argc = sizeof(argv) / sizeof(argv[0]);
	Configuration cfg;

	cfg.parseInputArguments(argc, argv);

	bool result = cfg.parsedOk();

	BOOST_CHECK_EQUAL(result, true);
}
BOOST_FIXTURE_TEST_CASE(parsedOk_emptyList_false, ConfigurationTest) {
	char* argv[] = {
		"Mood.exe"
	};
	int argc = sizeof(argv) / sizeof(argv[0]);
	Configuration cfg;

	cfg.parseInputArguments(argc, argv);

	bool result = cfg.parsedOk();

	BOOST_CHECK_EQUAL(result, false);
}
BOOST_AUTO_TEST_SUITE_END()
