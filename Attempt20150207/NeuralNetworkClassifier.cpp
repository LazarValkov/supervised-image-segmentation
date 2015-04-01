#include "NeuralNetworkClassiffier.h"
#include "BinaryConfusionMatrix.h"
#include "DataVisualiser.h"
#include <iostream>
#include <iomanip>
using namespace std;

int print_callback(FANN::neural_net &net, FANN::training_data &train,
	unsigned int max_epochs, unsigned int epochs_between_reports,
	float desired_error, unsigned int epochs, void *user_data);

NeuralNetworkClassifier::NeuralNetworkClassifier(string nnFilename) {
	nnetwork.create_from_file(nnFilename.c_str());
	inputVectorSize = nnetwork.get_num_input();
	outputVectorSize = nnetwork.get_num_output();
	wasLoadedFromFile = true;
}
/**Assuming only 1 hidden layer**/
NeuralNetworkClassifier::NeuralNetworkClassifier(int anInputVectorSize, int aNumOfHiddenUnits, int anOutputSize) :
inputVectorSize(anInputVectorSize), numHiddenUnits(aNumOfHiddenUnits), outputVectorSize(anOutputSize) {
	nnetwork.create_standard(3, inputVectorSize, numHiddenUnits, outputVectorSize);

	//debug: create deep learning nn with 2 hidden layers
	//nnetwork.create_standard(5, inputVectorSize, numHiddenUnits, 15, 6, outputVectorSize);
	
	nnetwork.set_learning_rate(learningRate);

	nnetwork.set_activation_steepness_hidden(1.0);
	nnetwork.set_activation_steepness_output(1.0);

	nnetwork.set_activation_function_hidden(FANN::SIGMOID_STEPWISE);
	nnetwork.set_activation_function_output(FANN::SIGMOID_STEPWISE);
}

void NeuralNetworkClassifier::train(const Mat& input_f, Mat& correctOutput_f) {
	cout << "BEGINING TO TRAIN THE NN" << endl;

	training_data trainingData;
	prepareTrainingData(trainingData, input_f, correctOutput_f);
	cout << "lenTrainingData: " << trainingData.length_train_data() << endl;
	
	if (!wasLoadedFromFile)
		nnetwork.init_weights(trainingData);
	nnetwork.set_callback(print_callback, NULL);
	nnetwork.train_on_data(trainingData, max_iterations,
		iterations_between_reports, desired_error);
}

ConfusionMatrix* NeuralNetworkClassifier::test(const Mat& input_f, const Mat& correctOutput_f, bool visualiseOutput) {
	// define classification output vector
	//Mat classificationResult = Mat(1, outputVectorSize, CV_32FC1);

	vector<int> resultsForVisualisation;

	training_data trainingData;
	prepareTrainingData(trainingData, input_f, correctOutput_f);

	int correctClassification = 0;
	BinaryConfusionMatrix *confusionMatrix = new BinaryConfusionMatrix();


	// perform classifier testing and report results
	Mat test_sample;
	int correct_class = 0;
	int wrong_class = 0;
	int false_positives[NUMBER_OF_CLASSES] = { 0 };

	int numberOfTestingSamples = trainingData.length_train_data();
	int a = 3;


	for (unsigned int i = 0; i < numberOfTestingSamples; ++i)
	{
		// Run the network on the test data
		fann_type *calc_out = nnetwork.run(trainingData.get_input()[i]);
		if (*calc_out >= 0.5f) {
			confusionMatrix->logPrediction(trainingData.get_output()[i][0], 1);
		}
		else {
			confusionMatrix->logPrediction(trainingData.get_output()[i][0], 0);
		}
		if (visualiseOutput)
			resultsForVisualisation.push_back(*calc_out >= 0.5f ? 2 : 0);
		//delete[] calc_out;
	}

	confusionMatrix->calculateOverallMeasures();
	cout << *confusionMatrix << endl;
	if (visualiseOutput)
		DataVisualiser::visualizeClassifiedFromBottomToTop(resultsForVisualisation, lastProcessedImgAnnotatedFilename);

	return confusionMatrix;
}

float NeuralNetworkClassifier::classifyImg(Mat& input_f, Mat& img, Mat& annotatedImg, Mat& output) {
	int cols = img.cols;
	int rows = img.rows;

	output.create(rows, cols, CV_32F);

	int currentEntry = 0;

	for (int j = 0; j < rows; ++j) {
		for (int i = 0; i < cols; ++i) {
			Vec3b annotatedImgIntensity = annotatedImg.at<Vec3b>(j, i);
			/**Add only offal pixels**/
			if (annotatedImgIntensity == GlobalVariables::COLOR_OFFAL_BACKGROUND)
				continue;

			output.at<float>(j, i) = classify(input_f, currentEntry);
			++currentEntry;
		}
	}

	return -1.;
}

float NeuralNetworkClassifier::classify(const Mat& input_f, int row) {
	int cols = input_f.cols;
	
	fann_type* input = new fann_type[cols];
	for (int i = 0; i < cols; ++i) {
		input[i] = input_f.at<float>(row, i);
	}
	
	fann_type* result = nnetwork.run(input);
	float fResult = *result;

	delete[] input;
	delete[] result;

	return fResult;
}

void NeuralNetworkClassifier::save(const string file) {
	nnetwork.save(file);
}

void NeuralNetworkClassifier::prepareTrainingData(training_data& trainingData, const Mat& input_f, const Mat& correctOutput_f) {
	int rows = input_f.rows;
	
	float **input = new float*[rows];
	float **output = new float*[rows];
	for (int j = 0; j < rows; j++) {
		input[j] = new float[inputVectorSize];
		output[j] = new float[outputVectorSize];
		for (int i = 0; i < inputVectorSize; i++) {
			input[j][i] = input_f.at<float>(j, i);
		}
		for (int i = 0; i < outputVectorSize; i++) {
			output[j][i] = correctOutput_f.at<float>(j, i);
		}
	}

	trainingData.set_train_data(rows, inputVectorSize, input, outputVectorSize, output);
	/**(for now) Delete the temporary arrays
	for (int j = 0; j < rows; j++) {
		delete[] input[j];
		delete[] output[j];
	}
	delete[] input;
	delete[] output;**/
}

int print_callback(FANN::neural_net &net, FANN::training_data &train,
	unsigned int max_epochs, unsigned int epochs_between_reports,
	float desired_error, unsigned int epochs, void *user_data)
{
	cout << "Epochs     " << setw(8) << epochs << ". "
		<< "Current Error: " << left << net.get_MSE() << right << endl;
	return 0;
}