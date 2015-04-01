#ifndef TESTER_H
#define TESTER_H
#include <iostream>
#include <string>
#include "Classifier.h"
#include "ImageAnalyser.h"
#include "GlobalVariables.h"
#include "ImageInfo.h"

using namespace std;

class Tester {
public:
	Tester(bool ttrainClassifier, bool lloadClassifierFromFile) 
		: trainClassifier(ttrainClassifier), loadClassifierFromFile(lloadClassifierFromFile) {}
	ConfusionMatrix* run(string logFilename, vector<ImageInfo*>& trainingImages, vector<ImageInfo*>& validationImages
		, int imgWidth, int imgHeight);
	void saveResults();


	/***Parameters which should be set by the overriding classes***/
	string description;
	string classifierFilename;
	Classifier::ClassifierType classifierType;
	ImageAnalyser::IA_Functions IAFunctions;
	int featureVectorSize_Input;
	int featureVectorSize_Output;
	GlobalVariables::Organ positiveOrgan;


	/*Purely Virtual Getters*/
	virtual Classifier* getClassifier() = 0;

protected:
	static const string classifiersStoreFolder;
	bool trainClassifier;
	bool loadClassifierFromFile;
};

#endif