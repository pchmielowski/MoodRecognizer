#pragma once
#include "Types.h"
#include "FeatureMatrixLoader.h"

class Mp3FeatureMatrixLoader : public FeatureMatrixLoader
{
public:
	Mp3FeatureMatrixLoader(const bool sdcEnabled) {}
	FeatureMatrix get(const FileName fileName) override;
	std::vector<double> loadAndDecodeMp3(const FileName fileName);
private:
	void calculateFeatureMatrix();
};
