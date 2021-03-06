#include "Mp3FeatureMatrixLoader.h"
#include "lame.h"
#include <vector>
using namespace std;

FeatureMatrix Mp3FeatureMatrixLoader::get(const FileName fileName)
{
	loadAndDecodeMp3(fileName);
	calculateFeatureMatrix();
	return FeatureMatrix();
}

std::vector<double> Mp3FeatureMatrixLoader::loadAndDecodeMp3(const FileName fileName)
{
	FILE* mp3File = fopen(fileName.c_str(), "rb");
	if (mp3File == NULL)
		throw std::runtime_error("Error reading MP3 file.");
	//fseek(mp3File, 0, SEEK_END);
	//long sizeMp3File = ftell(mp3FileContent);
	//fseek(mp3File, 0, SEEK_SET);

	lame_t lame = lame_init();
	lame_set_decode_only(lame, 1);
	if (lame_init_params(lame) == -1)
		throw std::runtime_error("Parameters failed to initialize properly in lame.");
	
	mp3data_struct mp3data;
	memset(&mp3data, 0, sizeof(mp3data));

	int numDataRead;
	vector<double> decodedMonoContent;

	const int PCM_BUFFER_SIZE = 4096;
	const int MP3_BUFFER_SIZE = PCM_BUFFER_SIZE;
	unsigned char mp3Buffer[MP3_BUFFER_SIZE];
	short int leftPcmBuffer[PCM_BUFFER_SIZE], rightPcmBuffer[PCM_BUFFER_SIZE];

	hip_t hip = hip_decode_init();
	while ((numDataRead = fread(mp3Buffer, sizeof(char), MP3_BUFFER_SIZE, mp3File)) > 0)
	{
		int numDataToDecode = numDataRead;
		int numDecodedSamples;
		do
		{
			numDecodedSamples = hip_decode1_headers(hip, mp3Buffer, numDataToDecode, leftPcmBuffer, rightPcmBuffer, &mp3data);
			for (int sampleIdx = 0; sampleIdx < numDecodedSamples; sampleIdx++)
			{
				decodedMonoContent.push_back(static_cast<double>(leftPcmBuffer[sampleIdx] + rightPcmBuffer[sampleIdx]) / 2 / 27772); // TODO: sprawdzi� jaka liczba
			}
			//numDataToDecode = 0;
		} while (numDecodedSamples>0);
	}

	hip_decode_exit(hip);
	lame_close(lame);
	fclose(mp3File);

	return decodedMonoContent;
}

void Mp3FeatureMatrixLoader::calculateFeatureMatrix()
{
	throw std::logic_error("The method or operation is not implemented.");
}
