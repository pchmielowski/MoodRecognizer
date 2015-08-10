#pragma once
#include "Ubm.h"
#include "Types.h"
#include <opencv\cv.h>
#include <string>
using namespace cv;

class UbmLoader
{
public:
	UbmLoader(FileName ubmFileName)
	{
		FileStorage fUBM;
		fUBM.open(ubmFileName, FileStorage::READ);

		fUBM["weights"] >> ubm_.weights_;
		ubm_.weights_.convertTo(ubm_.weights_, CV_32FC1);

		fUBM["means"] >> ubm_.means_;
		ubm_.means_.convertTo(ubm_.means_, CV_32FC1);
		ubm_.means_ = ubm_.means_.t();

		fUBM["numOfGaussComponents"] >> ubm_.numGaussComponents_;

		for (int componentIdx = 0; componentIdx < ubm_.numGaussComponents_; componentIdx++) {
			const int MAX_NUM_DIGITS = 4;
			char covNumber[MAX_NUM_DIGITS];
			_itoa(componentIdx, covNumber, 10);
			Mat cov;
			fUBM["cov"+string(covNumber)] >> cov;

			cov = cov.diag();
			cov.convertTo(cov, CV_32FC1);
			ubm_.covs_.push_back(cov);
		}

		fUBM.release();

		ubm_.createNormalDistribution(ubm_.means_.rows, ubm_.covs_);
	}
	virtual Ubm getUbm()
	{
		return ubm_;
	}
	virtual ~UbmLoader() {}
private:
	Ubm ubm_;
};
