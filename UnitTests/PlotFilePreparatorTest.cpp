#pragma warning(push, 0)
#include <boost/test/unit_test.hpp>
#include <fakeit.hpp>
#pragma warning(pop)
#include "FileWriter.h"
#include "PlotFilePreparator.h"
#include <vector>
using namespace fakeit;

class PlotFilePreparatorFixture {};

BOOST_FIXTURE_TEST_SUITE(PlotFilePreparatorTest, PlotFilePreparatorFixture)
BOOST_AUTO_TEST_CASE(write_emptyContent_correctCallsToWrite)
{
	Mock<FileWriter> fileWriter;
	When(Method(fileWriter, open)).Return();
	When(Method(fileWriter, appendContent)).Return();
	When(Method(fileWriter, saveAndClose)).Return();
	FileWriter& fileWriterInstance = fileWriter.get();

	PlotFilePreparator SUT(fileWriterInstance);

	SUT.write("someName");

	Verify(Method(fileWriter, open).Using("someName"));
	Verify(Method(fileWriter, saveAndClose));
}
BOOST_AUTO_TEST_CASE(addAlphasAndAccuracies_addOneAlphaOneAccuracy_correctContent)
{
	Mock<FileWriter> fileWriter;
	When(Method(fileWriter, open)).Return();
	When(Method(fileWriter, appendContent)).Return();
	When(Method(fileWriter, saveAndClose)).Return();
	FileWriter& fileWriterInstance = fileWriter.get();

	PlotFilePreparator SUT(fileWriterInstance);

	AlphaVector alphas = { .2f };
	SUT.addAlphas(alphas);
	std::vector<float> accuracies = { .9f };
	SUT.addAccuracies(accuracies);
	SUT.write("someName");

	Verify(Method(fileWriter, appendContent).Using("0.200 0.900;"));
}
BOOST_AUTO_TEST_CASE(addAlphasAndAccuracies_addOneAccuracyAndThenOneAlpha_correctContent)
{
	Mock<FileWriter> fileWriter;
	When(Method(fileWriter, open)).Return();
	When(Method(fileWriter, appendContent)).Return();
	When(Method(fileWriter, saveAndClose)).Return();
	FileWriter& fileWriterInstance = fileWriter.get();

	PlotFilePreparator SUT(fileWriterInstance);

	std::vector<float> accuracies = { .9f };
	SUT.addAccuracies(accuracies);
	AlphaVector alphas = { .2f };
	SUT.addAlphas(alphas);
	SUT.write("someName");

	Verify(Method(fileWriter, appendContent).Using("0.200 0.900;"));
}
BOOST_AUTO_TEST_CASE(addAlphasAndAccuracies_add4Alpha4Accuracy_correctContent)
{
	Mock<FileWriter> fileWriter;
	When(Method(fileWriter, open)).Return();
	When(Method(fileWriter, appendContent)).Return();
	When(Method(fileWriter, saveAndClose)).Return();
	FileWriter& fileWriterInstance = fileWriter.get();

	PlotFilePreparator SUT(fileWriterInstance);

	AlphaVector alphas = { .2f, .7f, .1f, 1.f };
	SUT.addAlphas(alphas);
	std::vector<float> accuracies = { .9f, .3f, .2f, 0.f };
	SUT.addAccuracies(accuracies);
	SUT.write("someName");

	Verify(Method(fileWriter, appendContent).Using("0.200 0.900;\n0.700 0.300;\n0.100 0.200;\n1.000 0.000;"));
}
BOOST_AUTO_TEST_CASE(write_realFile)
{
	FileWriter fileWriterInstance;
	PlotFilePreparator SUT(fileWriterInstance);

	AlphaVector alphas = { .2f, .7f, .1f, 1.f };
	SUT.addAlphas(alphas);
	std::vector<float> accuracies = { .9f, .3f, .2f, 0.f };
	SUT.addAccuracies(accuracies);
	SUT.write("toDelete.txt");
}
BOOST_AUTO_TEST_SUITE_END()