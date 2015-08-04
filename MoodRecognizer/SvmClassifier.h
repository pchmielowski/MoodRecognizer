#pragma once;
#include "SuperVectorCalculator.h"
#include "Types.h"

class MoodsInterface;
class InputFileNames;
class Moods;

class SvmClassifier {
public:
	SvmClassifier(SuperVectorCalculator& superVectorCalculator, FileName svmModelFileName);
	void train(MoodsInterface& moods, InputFileNames& inputFileNames);
	Moods predictMoods(InputFileNames& inputFileNames);
	//SuperVectors reduce(SuperVectors superVectorsToReduce);
	void trainPca(SuperVectors);
private:
	InputFileNames* inputFileNames_;
	SuperVectorCalculator* superVectorCalculator_;
};
