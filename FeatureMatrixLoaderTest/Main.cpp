#include "Mp3FeatureMatrixLoader.h"
#include <string>
#include "sndfile.h"
#include <iostream>

using namespace std;

class Mp3FeatureMatrixLoaderTest {
public:
	void SaveWaveFileFromMp3(string pathToInputFile, string pathToOutputFile) {
		Mp3FeatureMatrixLoader SUT(false);
		try {
			wave_ = SUT.loadAndDecodeMp3(pathToInputFile);
		}
		catch (std::exception e) {
			cout << e.what() << endl;
		}

		const int BUFFER_LENGTH = wave_.size();
		double* buffer = new double[BUFFER_LENGTH];
		for (auto sample = wave_.begin(); sample != wave_.end(); ++sample) {
			int sampleIdx = sample - wave_.begin();
			buffer[sampleIdx] = *sample;
		}

		SNDFILE* outputFile = NULL;
		SF_INFO outputFileInfo;
		outputFileInfo.channels = 1;
		outputFileInfo.samplerate = 44100;
		outputFileInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
		outputFile = sf_open(pathToOutputFile.c_str(), SFM_WRITE, &outputFileInfo);
		sf_write_double(outputFile, buffer, BUFFER_LENGTH);
		sf_close(outputFile);
	}
private:
	std::vector<double> wave_;
};

int main() {
	Mp3FeatureMatrixLoaderTest mp3FeatureMatrixLoaderTest;
	const string PATH = "C:\\OneDrive\\C_MoodRecognizer\\FeatureMatrixLoaderTest\\mp3tests\\";
	mp3FeatureMatrixLoaderTest.SaveWaveFileFromMp3(PATH + "untitled.mp3", PATH + "audio.wav");

	return 0;
}