#include "SuperVectorCalculator.h"
#include "Types.h"
#include "opencv/cv.h"

using namespace cv;

SuperVector SuperVectorCalculator::calculate(FileName fileName)
{
	FeatureMatrix featureMatrix = featureMatrixLoader_->get(fileName);

	double* wp_t = NULL;
	double** Pr = new double*[featureMatrix.rows];
	double B_i, sum_wp_t;
	Mat A_i, E_i, mu_i;
	int numGaussComponents = ubm_->getNumGaussComponents();
	for (int t = 0; t < featureMatrix.rows; t++) // iteracja po kolejnych ramkach MFCC
	{
		Pr[t] = new double[numGaussComponents];
		wp_t = new double[numGaussComponents];
		sum_wp_t = 0;

		for (int i = 0; i < numGaussComponents; i++) {
			wp_t[i] = ubm_->weights.at<double>(i) *					// Eq. 3 - licznik (skalar)
				probability(featureMatrix.row(t), ubm_->means.row(i), ubm_->covs.at(i), i);
			sum_wp_t += wp_t[i];										// Eq. 3 - mianownik (skalar)
			// TODO: podczas optymalizacji sprawdziæ
			// czy da siê omin¹æ =+, 
			// ¿eby b³êdy numeryczne siê nie akumulowa³y tak bardzo
		}
		for (int i = 0; i < numGaussComponents; i++)
			Pr[t][i] = wp_t[i] / sum_wp_t;								// Eq. 3 - dzielenie (skalar)
		delete[] wp_t;
	}

	SuperVector superVector;
	for (int componentIdx = 0; componentIdx < numGaussComponents; ++componentIdx)
	{
		A_i = Mat::zeros(1, ubm_->n_of_mfcc_coef, CV_64F);			// A_i - 1 x n_of_mfcc_coef wektor
		B_i = 0;														// B_i - skalar

		for (int t = 0; t < featureMatrix.rows; t++) {								// iteracja po kolejnych ramkach MFCC
			A_i += Pr[t][componentIdx] * featureMatrix.row(t);									// Eq. 2 - licznik
			B_i += Pr[t][componentIdx];											// Eq. 2 - mianownik
		}
		E_i = A_i / B_i;												// Eq. 2 - dzielenie

		addWeighted(E_i, alpha, ubm_->means.row(componentIdx), 1.0 - alpha, 0.0, mu_i); // Eq. 1

		if (superVector.empty())
			superVector = mu_i.clone();
		else
			hconcat(superVector, mu_i, superVector);
	}

	for (int t = 0; t < featureMatrix.rows; t++)
		delete[] Pr[t];
	delete[] Pr;

	return superVector;
}

SuperVectorCalculator::SuperVectorCalculator(FeatureMatrixLoader& featureMatrixLoader, vector<Alpha> alpha, vector<int> numComponents, Ubm& ubm)
{

}

double probability(const Mat& x, const Mat& mean, const Mat& covariance, int gaus_nmbr) const {
	// zwraca prawdopodobieñstwo wektora x przy rozk³adzie N(mean, covariance)
	// normal_distribution<>* norm_distr;

	double prob = 1;

	for (int i = 0; i < pTheUBM->n_of_mfcc_coef; i++) {

		prob *= pdf(pTheUBM->norm_distr[gaus_nmbr][i], x.at<double>(0, i));
		// prob *= pdf(pTheUBM->norm_distr[gaus_nmbr][i], x.at<double>(0, i));
		//delete norm_distr;
	}
	return prob;
}