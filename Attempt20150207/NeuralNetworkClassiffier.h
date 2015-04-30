#ifndef NEURAL_NETWORK_CLASSIFIER
#define NEURAL_NETWORK_CLASSIFIER
#include "Classifier.h"
#include "fann/floatfann.h"
#include "fann/fann_cpp.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "GlobalVariables.h"

using namespace FANN;
using namespace cv;

class NeuralNetworkClassifier : public Classifier {
public:
	NeuralNetworkClassifier(string nnFile);
	/**Assuming only 1 hidden layer**/
	NeuralNetworkClassifier(int i, int hiddenUnits, int o);

	virtual void train(const Mat& input_f, Mat& correctOutput_f);
	virtual ConfusionMatrix*  test(const Mat& input_f, const Mat& correctOutput_f);
	virtual float classify(const Mat& input_f, int row);
	//DEBUG. TODO: Find a better way!
	virtual float classifyImg(Mat& input_f, Mat& img, Mat &annotatedImg, Mat& output);

	virtual void save(const string file);


	/**Modifiable parameters**/
	unsigned int max_iterations = 500;
	int inputVectorSize, numHiddenUnits, outputVectorSize;
	float desired_error = 0.001f;
private:
	
	void prepareTrainingData(training_data& trainingData
		, const Mat& input_f, const Mat& correctOutput_f);
	
	bool wasLoadedFromFile = false;
	neural_net nnetwork;
	
	
	const unsigned int iterations_between_reports = 10;
	const float learningRate = 0.7f;
};
#endif