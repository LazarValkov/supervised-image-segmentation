#ifndef BOTTOMS_UP_H
#define BOTTOMS_UP_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Classifier.h"
#include "NeuralNetworkClassiffier.h"
#include "ImageAnalyser.h"
#include "GlobalVariables.h"
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

class BottomsUp {
public:
	//ORDER: OTHERS, HEART, LUNGS, DIAPHRAGM, LIVER
	BottomsUp(string* classifierFilenames, int numOfClassifiers) : _numOfClassifiers(numOfClassifiers){
		_classifiers = new Classifier*[_numOfClassifiers];
		for (int i = 0; i < numOfClassifiers; ++i) {
			_classifiers[i] = new NeuralNetworkClassifier(classifierFilenames[i]);
		}
	}
	~BottomsUp() {
		for (int i = 0; i < _numOfClassifiers; ++i) {
			delete _classifiers[i];
		}
		delete[] _classifiers;
	}

	void run(string filename_imgList_train, string filename_imgList_test, ImageAnalyser::IA_Functions iaFunctions, int featureVectorSize_Input, int featureVectorSize_Output) {

		/**Get normalisationParameters using the training data**/
		Mat *input_f_train, *correctOutput_f_train;
		input_f_train = new Mat;
		correctOutput_f_train = new Mat;

		/**Run Image Analysis**/
		/*ImageAnalyser::prepareClassificationData(*input_f_train, *correctOutput_f_train, iaFunctions
		, filename_imgList_train, true, featureVectorSize_Input
			, featureVectorSize_Output, GlobalVariables::LIVER);*/
		float ** normalisationParameters = ImageAnalyser::calculateNormalizationParameters(*input_f_train);
		delete input_f_train;
		delete correctOutput_f_train;

		/**********************TEST*************************/
		input_f_test = new Mat;
		Mat* correctOutput_f_test = new Mat;
		/*
		ImageAnalyser::prepareClassificationData(*input_f_test, *correctOutput_f_test, iaFunctions
			, filename_imgList_test, true, featureVectorSize_Input
			, featureVectorSize_Output, GlobalVariables::LIVER);*/
		//Using the normalisationParameters calculated on the training data
		ImageAnalyser::normaliseData(*input_f_test, normalisationParameters);
		delete correctOutput_f_test;

		
	}

	void performClassificiation(string imgFilename, string imgAnnotatedFilename) {

		int threshold_elseId = 10;
		int threshold_uid = 15;

		Mat img = imread(imgFilename);
		Mat imgAnnotated = imread(imgAnnotatedFilename);

		int rows = img.rows;
		int cols = img.cols;


		Mat output_liver;
		liverClassifier->classifyImg(*input_f_test, img, imgAnnotated, output_liver);

		for (int i = 0; i < cols; ++i){
			for (int j = rows - 1; j > -1; --j) {
				//How many pixels have not been identified
				int uid_count = 0;
				int uid_start = -1;
				//How many pixels have been identified as something else in a row
				int elseId = 0;
				int elseId_start = -1;

				
			}
		}
	}
private:
	Mat * input_f_test;
	int* currentlyAt;
	int cols;

	int _numOfClassifiers;
	Classifier** _classifiers;
};




#endif