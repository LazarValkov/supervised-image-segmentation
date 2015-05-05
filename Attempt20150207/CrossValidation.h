#ifndef CROSS_VALIDATION_H
#define CROSS_VALIDATION_H

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "ImageInfo.h"
#include <iostream>
#include "Tester.h"

using namespace std;
class CrossValidation {
public:
	int numOfListedImages, imagesWidth, imagesHeight;

	void kfold(string logFilename, string testName, int k, string fileList, Tester* tester) {
		vector<ImageInfo*> allImgInfos;
		populateVectorFromFile(allImgInfos, fileList);
		random_shuffle(allImgInfos.begin(), allImgInfos.end());

		int blockSize = numOfListedImages / k;

		cout << "BEGINING K-FOLD cross validation" << endl;

		//ConfusionMatrix *a = new ConfusionMatrix[k];
		vector<ConfusionMatrix*> resultingConfusionMatrices;


		for (int i = 0; i < k; ++i) {
			cout << "Starting K-FOLD Iteration:" << i << endl;

			int validationBlockInd_start = 0;
			int validationBlockInd_end = 0;
			
			//handle the special case where k=1:
			//set the last two images to be validation
			if (k == 1) {
				validationBlockInd_start = numOfListedImages - 10;
				validationBlockInd_end = numOfListedImages; //exclusive
			}
			else {
				validationBlockInd_start = i*blockSize;
				validationBlockInd_end = (i + 1)*blockSize; //exclusive
			}

			/**Sort out the training/validation sets for the current interation**/
			vector<ImageInfo*> trainingImages, validationImages;
			for (int x = 0; x < numOfListedImages; ++x) {
				if (x >= validationBlockInd_start  && x < validationBlockInd_end) {
					validationImages.push_back(allImgInfos[x]);
				}
				else {
					trainingImages.push_back(allImgInfos[x]);
				}
			}

			resultingConfusionMatrices.push_back(
				tester->run(logFilename, trainingImages, validationImages, imagesWidth, imagesHeight)
				);
		}

		/***Calculate standard deviation ***/
		float sum_accuracy = 0;
		float sum_precision = 0;
		float sum_truePositive = 0;
		float sum_fMeasure = 0;
		int numberOfConfMatrices = resultingConfusionMatrices.size();
		for (int i = 0; i < resultingConfusionMatrices.size(); ++i) {
			ConfusionMatrix* tmpCM = resultingConfusionMatrices[i];
			sum_accuracy += tmpCM->getAccuracy();
			sum_precision += tmpCM->getPrecision();
			sum_truePositive += tmpCM->getTruePositive();
			sum_fMeasure += tmpCM->getFMeasure();
		}
		
		float mean_accuracy = sum_accuracy / numberOfConfMatrices;
		float mean_precision = sum_precision / numberOfConfMatrices;
		float mean_truePositive = sum_truePositive / numberOfConfMatrices;
		float mean_fMeasure = sum_fMeasure / numberOfConfMatrices;

		//todo: debug and check this works!:
		sum_accuracy = sum_precision = sum_truePositive = sum_fMeasure = 0;

		for (int i = 0; i < resultingConfusionMatrices.size(); ++i) {
			ConfusionMatrix* tmpCM = resultingConfusionMatrices[i];
			sum_accuracy += pow((tmpCM->getAccuracy()-mean_accuracy),2);
			sum_precision += pow((tmpCM->getPrecision()-mean_precision),2);
			sum_truePositive += pow((tmpCM->getTruePositive()-mean_truePositive),2);
			sum_fMeasure += pow((tmpCM->getFMeasure() - mean_fMeasure), 2);
		}
		
		
		float std_deviation_accuracy = sqrt(sum_accuracy / numberOfConfMatrices);
		float std_deviation_precision = sqrt(sum_precision / numberOfConfMatrices);
		float std_deviation_truePositive = sqrt(sum_truePositive / numberOfConfMatrices);
		float std_deviation_fMeasure = sqrt(sum_fMeasure / numberOfConfMatrices);

		cout << "Accuracy Mean\StdDeviation: " << mean_accuracy << " \ " << std_deviation_accuracy << endl;
		cout << "Precision Mean\StdDeviation: " << mean_precision << " \ " << std_deviation_precision << endl;
		cout << "TruePositive Mean\StdDeviation: " << mean_truePositive << " \ " << std_deviation_truePositive << endl;
		cout << "FMeasure Mean\StdDeviation:" << mean_fMeasure << " \ " << std_deviation_fMeasure << endl;

		//Log the results to file
		ofstream ofs;
		ofs.open(logFilename, ios::app);
		if (!ofs)
			ofs.open(logFilename);
		ofs << testName << endl;
		ofs << resultingConfusionMatrices.size();
		for (int i = 0; i < resultingConfusionMatrices.size(); ++i) {
			ConfusionMatrix* tmpCM = resultingConfusionMatrices[i];
			ofs << " " << tmpCM->getAccuracy() << " " << tmpCM->getFMeasure();
		}
		ofs << " " << mean_accuracy << " " << std_deviation_accuracy <<  " " << mean_fMeasure << " "<< std_deviation_fMeasure << endl;
		//ofs << " " << testResults->getAccuracy() << " " << testResults->getFMeasure();
		//ofs << description << "\n" << *testResults << "\n";
		ofs.close();
	}

	void populateVectorFromFile(vector<ImageInfo*>& allImgInfos, string fileList) {

		ifstream ifs(fileList);

		/*Find the number of listed images, width, height*/
		string totalImagesCountStr, imgWidth, imgHeight;
		getline(ifs, totalImagesCountStr); getline(ifs, imgWidth); getline(ifs, imgHeight);
		numOfListedImages = atoi(totalImagesCountStr.c_str());
		imagesWidth = atoi(imgWidth.c_str());
		imagesHeight = atoi(imgHeight.c_str());


		string imageFilename, annotatedImageFilename,
			scountOthers, scountHeart, scountLungs, scountDiaphragm, scountLiver;
		int countOthers, countHeart, countLungs, countDiaphragm, countLiver;

		while (getline(ifs, imageFilename) && getline(ifs, annotatedImageFilename)
			&& getline(ifs, scountOthers) && getline(ifs, scountHeart)
			&& getline(ifs, scountLungs) && getline(ifs, scountDiaphragm) && getline(ifs, scountLiver))
		{
			countOthers = atoi(scountOthers.c_str()); countHeart = atoi(scountHeart.c_str());
			countLungs = atoi(scountLungs.c_str()); countDiaphragm = atoi(scountDiaphragm.c_str());
			countLiver = atoi(scountLiver.c_str());
			
			ImageInfo * newImgInfo = new ImageInfo(imageFilename, annotatedImageFilename
				, countOthers, countHeart, countLungs, countDiaphragm, countLiver);
			allImgInfos.push_back(newImgInfo);
		}
	}
};

#endif