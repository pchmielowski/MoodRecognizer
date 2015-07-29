//#include "Mp3FeatureMatrixLoader.h"
#include "MatFeatureMatrixLoader.h"
#include <string>
#include "sndfile.h"
#include <iostream>

using namespace std;

//class Mp3FeatureMatrixLoaderTest {
//public:
//	void SaveWaveFileFromMp3() {
//		string pathToInputFile = PATH + "untitled.mp3";
//		string pathToOutputFile = PATH + "audio.wav";
//		Mp3FeatureMatrixLoader SUT(false);
//		try {
//			wave_ = SUT.loadAndDecodeMp3(pathToInputFile);
//		}
//		catch (std::exception e) {
//			cout << e.what() << endl;
//		}
//
//		const int BUFFER_LENGTH = wave_.size();
//		double* buffer = new double[BUFFER_LENGTH];
//		for (auto sample = wave_.begin(); sample != wave_.end(); ++sample) {
//			int sampleIdx = sample - wave_.begin();
//			buffer[sampleIdx] = *sample;
//		}
//
//		SNDFILE* outputFile = NULL;
//		SF_INFO outputFileInfo;
//		outputFileInfo.channels = 1;
//		outputFileInfo.samplerate = 44100;
//		outputFileInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
//		outputFile = sf_open(pathToOutputFile.c_str(), SFM_WRITE, &outputFileInfo);
//		sf_write_double(outputFile, buffer, BUFFER_LENGTH);
//		sf_close(outputFile);
//	}
//private:
//	std::vector<double> wave_;
//	static const string PATH = "C:\\OneDrive\\C_MoodRecognizer\\FeatureMatrixLoaderTest\\mp3tests\\";
//};

int main() {
	//Mp3FeatureMatrixLoaderTest mp3FeatureMatrixLoaderTest;
	//mp3FeatureMatrixLoaderTest.SaveWaveFileFromMp3();

	MatFeatureMatrixLoader SUT(false);

	cv::Mat result;
	result = SUT.get("C:\\OneDrive\\C_MoodRecognizer\\UnitTests\\matrixForTest\\matrix.mat");

	cout << result << endl;

	return 0;
}

//#include <opencv\cv.h>
//#include <fakeit.hpp>
//#include "MatFeatureMatrixLoader.h"
//#pragma warning(pop)
//using namespace fakeit;
//using namespace std;
