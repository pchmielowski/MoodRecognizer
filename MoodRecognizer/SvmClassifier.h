#pragma once;
#include "SuperVectorCalculator.h"
#include "Types.h"

class Moods;
class InputFileNames;

class SvmClassifier {
public:
	SvmClassifier(SuperVectorCalculator& superVectorCalculator, FileName svmModelFileName);
	void train(Moods& moods, InputFileNames& inputFileNames);
	Moods predictMoods(InputFileNames& inputFileNames);

	SuperVectors reduce(SuperVectors superVectorsToReduce);
};
