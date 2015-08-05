#pragma once;
#include "SuperVectorCalculator.h"
#include "Types.h"
#include "SvmClassifier.h"

class MoodsInterface;
class InputFileNames;
class Moods;

class SuperVectorCollector {
public:
	SuperVectorCollector(SuperVectorCalculator& superVectorCalculator, SvmClassifier& svmClassifier);
	void train(MoodsInterface& moods, InputFileNames& inputFileNames);
	Moods predictMoods(InputFileNames& inputFileNames);
private:
	InputFileNames* inputFileNames_;
	SuperVectorCalculator* superVectorCalculator_;
};
