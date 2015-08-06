#pragma once;
#include "SuperVectorCalculator.h"
#include "Types.h"
#include "SvmClassifier.h"

class MoodsInterface;
class InputFileNames;
class Moods;
class PcaReductor;

class SuperVectorCollector {
public:
	SuperVectorCollector(SuperVectorCalculator& superVectorCalculator, PcaReductor& pcaReductor,
		SvmClassifier& svmClassifier, const AlphaVector alphas);
	void train(MoodsInterface& moods, InputFileNames& inputFileNames);

	Moods predictMoods(InputFileNames& inputFileNames);
private:
	InputFileNames* inputFileNames_ = nullptr;
	SuperVectorCalculator* superVectorCalculator_ = nullptr;
	PcaReductor* pcaReductor_ = nullptr;
	AlphaVector alphas_;
};