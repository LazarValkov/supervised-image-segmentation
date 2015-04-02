#include "BinaryAdaBoostClassifier.h"
#include "BinaryConfusionMatrix.h"
#include <iostream>

using namespace std;

BinaryAdaBoostClassifier::BinaryAdaBoostClassifier(string fileName) {
	boost.load(fileName.c_str());
}
BinaryAdaBoostClassifier::BinaryAdaBoostClassifier() {

}

void BinaryAdaBoostClassifier::train(const Mat& input_f, Mat& correctOutput_f) {
	/*make the output from [0,1] to [-1,1]*/
	int rows = correctOutput_f.rows;
	int cols = correctOutput_f.cols;
	for (int j = 0; j < rows; ++j) {
		for (int i = 0; i < cols; ++i) {
			if (correctOutput_f.at<float>(j, i) < 0.5f)
				correctOutput_f.at<float>(j, i) = -1;
		}
	}

	boost.train(input_f, CV_ROW_SAMPLE, correctOutput_f);
}

ConfusionMatrix*  BinaryAdaBoostClassifier::test(const Mat& input_f, const Mat& correctOutput_f) {
	BinaryConfusionMatrix *confusionMatrix = new BinaryConfusionMatrix();

	int rows = correctOutput_f.rows;
	int cols = correctOutput_f.cols;
	Mat test_sample;
	for (int j = 0; j < rows; ++j) {
		// extract a row from the testing matrix
		test_sample = input_f.row(j);
		float prediction = boost.predict(test_sample);
		if (prediction < 0.0f)
			confusionMatrix->logPrediction(correctOutput_f.at<float>(j, 0), 0);
		else
			confusionMatrix->logPrediction(correctOutput_f.at<float>(j, 0), 1);
	}
	confusionMatrix->calculateOverallMeasures();
	cout << *confusionMatrix << endl;
	return confusionMatrix;
}

void BinaryAdaBoostClassifier::save(const string file) {
	
}