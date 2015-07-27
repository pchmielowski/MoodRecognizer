#define BOOST_TEST_MODULE Test
#define _CRT_SECURE_NO_WARNINGS
#include "boost\test\included\unit_test.hpp"
#include <fstream>
// My classes:
#include "Configuration.h"
#include "Types.h"
#define _CRT_SECURE_NO_WARNINGS

class ConfigurationTest {
public:
	std::string configurationFileContent;
	void saveConfigurationFile(FileName configurationFileName, std::string configurationFileContent) {
		std::ofstream configurationFile;
		configurationFile.open(configurationFileName, std::ios::trunc);
		configurationFile << configurationFileContent;
		configurationFile.close();
	}
};

BOOST_AUTO_TEST_SUITE(ConfigurationTestSuite)
BOOST_FIXTURE_TEST_CASE(Configuration_parseInputArguments_parseCorrectlyFilenames, ConfigurationTest) {
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

BOOST_FIXTURE_TEST_CASE(Configuration_parseInputArguments_parseCorrectlyMode, ConfigurationTest) {
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
BOOST_FIXTURE_TEST_CASE(Configuration_parseInputArguments_xSwitch_throwsRuntimeErrorException, ConfigurationTest) {

	char* argv[] = {
		"Mood.exe",
		"-x", "someText"
	};
	int argc = sizeof(argv) / sizeof(argv[0]);
	Configuration cfg;

	BOOST_REQUIRE_THROW(cfg.parseInputArguments(argc, argv), std::runtime_error);
}
BOOST_FIXTURE_TEST_CASE(Configuration_parseInputArguments_noSvmFile_throwsRuntimeErrorException, ConfigurationTest) {

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

BOOST_FIXTURE_TEST_CASE(Configuration_parseConfigurationFile_getValue, ConfigurationTest) {

	//float alpha = (float).32;
	//int numOfComponents = 12;

	//configurationFileContent = "<alpha range=false>\n";
	//configurationFileContent += "<value>" + std::to_string(alpha) + "</value>\n";
	//configurationFileContent += "</alpha>\n";
	//configurationFileContent += "<componentNumber range=false>\n";
	//configurationFileContent += "<min>" + std::to_string(1) + "</min>\n";
	//configurationFileContent += "<max>" + std::to_string(3) + "</max>\n";
	//configurationFileContent += "</componentNumber>";

	//saveConfigurationFile(s_configurationFileName, configurationFileContent);

	//cfg.parseConfigurationFile(s_configurationFileName);

	//BOOST_CHECK_EQUAL(cfg.getAlpha(), alpha);
	//BOOST_CHECK_EQUAL(cfg.getNumComponents(), numOfComponents);
}
BOOST_FIXTURE_TEST_CASE(Configuration_parseConfigurationFile_getRange, ConfigurationTest) {}
BOOST_FIXTURE_TEST_CASE(Configuration_parseConfigurationFile_getValueWhenRangeIsPresent, ConfigurationTest) {

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
BOOST_FIXTURE_TEST_CASE(Configuration_parseConfigurationFile_wrongCfgFileException, ConfigurationTest) {  }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AnotherSuite)
BOOST_AUTO_TEST_CASE(AnyCase) {
	BOOST_CHECK_EQUAL(1, 1);
}
BOOST_AUTO_TEST_SUITE_END()