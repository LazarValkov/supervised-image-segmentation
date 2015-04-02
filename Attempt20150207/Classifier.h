#ifndef MY_CLASSIFIER_H
#define MY_CLASSIFIER_H
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


#include "ConfusionMatrix.h"

using namespace cv;

class Classifier {
public:

	virtual void train(const Mat& input_f, Mat& correctOutput_f) = 0;
	virtual ConfusionMatrix* test(const Mat& input_f, const Mat& correctOutput_f) = 0;
	virtual float classify(const Mat& input_f, int row) = 0;
	virtual float classifyImg(Mat& input_f, Mat& img, Mat &annotatedImg, Mat& output) { return -1.; }
	virtual void save(const string file) = 0;

	enum ClassifierType { NeuralNetwork };

	string lastProcessedImgAnnotatedFilename;
};
#endif