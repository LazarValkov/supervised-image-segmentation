#include "Tester.h"
#include "NeuralNetworkClassiffier.h"
#include <fstream>


ConfusionMatrix* Tester::run(string logFilename, vector<ImageInfo*>& trainingImages, vector<ImageInfo*>& validationImages
	,int imgWidth, int imgHeight) {
	
	
	Classifier*classifier = getClassifier();

	/*********************TRAIN*************************/
	
	Mat *input_f_train, *correctOutput_f_train;
	input_f_train = new Mat;
	correctOutput_f_train = new Mat;

	/**Run Image Analysis**/
	ImageAnalyser::prepareClassificationData(*input_f_train, *correctOutput_f_train, IAFunctions
		, trainingImages, imgWidth, imgHeight, true, featureVectorSize_Input
		, featureVectorSize_Output, positiveOrgan, negativeOrgans);
	float ** normalisationParameters = ImageAnalyser::calculateNormalizationParameters(*input_f_train);
	if (trainClassifier) {
		ImageAnalyser::normaliseData(*input_f_train, normalisationParameters);
		ImageAnalyser::shuffleMatrixRows(*input_f_train, *correctOutput_f_train);
		classifier->train(*input_f_train, *correctOutput_f_train);
	}
	delete input_f_train;
	delete correctOutput_f_train;
	classifier->save(classifierFilename);
	/**********************TEST*************************/
	Mat* input_f_test = new Mat;
	Mat* correctOutput_f_test = new Mat;
	
	/**Run Image Analysis**/
	ImageAnalyser::prepareClassificationData(*input_f_test, *correctOutput_f_test, IAFunctions
		, validationImages, imgWidth, imgHeight, false, featureVectorSize_Input
		, featureVectorSize_Output, positiveOrgan, negativeOrgans);
	//Using the normalisationParameters calculated on the training data
	ImageAnalyser::normaliseData(*input_f_test, normalisationParameters);


	ConfusionMatrix* testResults = classifier->test(*input_f_test, *correctOutput_f_test);
	

	for (int i = 0; i < input_f_test->cols; ++i)
		delete[] normalisationParameters[i];
	delete[] normalisationParameters;
	delete input_f_test;
	delete correctOutput_f_test;


	/**Append the test results to the log file**/
	ofstream ofs;
	ofs.open(logFilename, ios::app);
	if (!ofs)
		ofs.open(logFilename);
	ofs << description << "\n" << *testResults << "\n";
	ofs.close();

	return testResults;
}

const string Tester::classifiersStoreFolder = "C:\\temp\\";