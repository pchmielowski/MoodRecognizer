#pragma once;
#include "FileWriter.h"
#include "Types.h"
#include <string>
#include <sstream>
using namespace std;

class PlotFilePreparator
{
public:
	PlotFilePreparator(FileWriter& fileWriter)
	{
		fileWriter_ = &fileWriter;
	}
	void addAlphas(AlphaVector alphas)
	{
		if (aa.empty())
		{
			for (auto alpha : alphas)
			{
				aa.push_back({ alpha, .9 });
			}
		}
		else
		{
			assert(aa.size() == alphas.size());
			for (int accuracyIdx = 0; accuracyIdx < alphas.size(); ++accuracyIdx)
			{
				aa[accuracyIdx].alpha = alphas[accuracyIdx];
			}
		}
	}
	void addAccuracies(vector<float> accuracies)
	{
		if (aa.empty())
		{
			for (auto accuracy : accuracies)
			{
				aa.push_back({ 0.0f, accuracy });
			}
		}
		else
		{
			assert(aa.size() == accuracies.size());
			for (int accuracyIdx = 0; accuracyIdx < accuracies.size(); ++accuracyIdx)
			{
				aa[accuracyIdx].accuracy = accuracies[accuracyIdx];
			}
		}
	}
	void write(const FileName fileName)
	{
		assert(fileWriter_ != nullptr);
		string content = "";
		int lineIdx = 0;
		for (auto alphaAccuracy : aa)
		{
			stringstream stream;
			stream << fixed << setprecision(3) << alphaAccuracy.alpha;
			stream << " ";
			stream << fixed << setprecision(3) << alphaAccuracy.accuracy;
			stream << ";";

			content += stream.str();
			bool isLastLine = lineIdx == (aa.size() - 1);
			if (!isLastLine)
				content += "\n";
			++lineIdx;
		}
		fileWriter_->appendContent(content);
		assert(fileWriter_ != nullptr);
		fileWriter_->open(fileName);
		fileWriter_->saveAndClose();
	}
private:
	FileWriter* fileWriter_ = nullptr;
	AlphasAccuracies aa; // TODO RENAME
};