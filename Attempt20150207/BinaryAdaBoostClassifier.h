#ifndef BINARY_ADABOOST_CLASSIFIER
#define BINARY_ADABOOST_CLASSIFIER
#include "Classifier.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2\ml\ml.hpp"

using namespace cv;

class BinaryAdaBoostClassifier : public Classifier {
public:
	BinaryAdaBoostClassifier(string fileName);
	BinaryAdaBoostClassifier();

	virtual void train(const Mat& input_f, Mat& correctOutput_f);
	virtual ConfusionMatrix*  test(const Mat& input_f, const Mat& correctOutput_f);
	virtual void save(const string file);
	virtual float classify(const Mat& input_f, int row) { return -1; }

	/**Modifiable parameters**/
	
private:
	bool wasLoadedFromFile = false;
	CvBoost boost;
};
#endif