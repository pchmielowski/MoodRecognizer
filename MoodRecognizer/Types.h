#pragma once
#include <opencv\cv.h>
#include <string>
#include <vector>

typedef std::string					FileName;
typedef cv::Mat						FeatureMatrix; 
typedef float						Alpha;
typedef std::vector<Alpha>			AlphaVector;
typedef int							Mood;
typedef std::vector<Mood>			MoodsVector;
typedef cv::Mat						SuperVector;
typedef std::vector<SuperVector>	SuperVectors;

struct alphaAccuracy
{
	Alpha alpha;
	float accuracy;
};
typedef std::vector<alphaAccuracy>	AlphasAccuracies; // TODO: pomyœleæ nad nazw¹