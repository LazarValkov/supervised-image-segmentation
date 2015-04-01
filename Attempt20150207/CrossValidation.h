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

	void kfold(int k, string logFilename, string fileList, Tester* tester) {
		vector<ImageInfo*> allImgInfos;
		populateVectorFromFile(allImgInfos, fileList);
		random_shuffle(allImgInfos.begin(), allImgInfos.end());

		int blockSize = numOfListedImages / k;

		cout << "BEGINING K-FOLD cross validation" << endl;

		//ConfusionMatrix *a = new ConfusionMatrix[k];
		vector<ConfusionMatrix*> resultingConfusionMatrices;


		for (int i = 0; i < k; ++i) {
			cout << "Starting K-FOLD Iteration:" << i << endl;
			int validationBlockInd_start = i*blockSize;
			int validationBlockInd_end = (i+1)*blockSize; //exclusive
			
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
		int numberOfConfMatrices = resultingConfusionMatrices.size();
		for (int i = 0; i < resultingConfusionMatrices.size(); ++i) {
			ConfusionMatrix* tmpCM = resultingConfusionMatrices[i];
			sum_accuracy += tmpCM->getAccuracy();
			sum_precision += tmpCM->getPrecision();
			sum_truePositive += tmpCM->getTruePositive();
		}
		float mean_accuracy = sum_accuracy / numberOfConfMatrices;
		float mean_precision = sum_precision / numberOfConfMatrices;
		float mean_truePositive = sum_truePositive / numberOfConfMatrices;

		//todo: debug and check this works!:
		sum_accuracy = sum_precision = sum_truePositive = 0;

		for (int i = 0; i < resultingConfusionMatrices.size(); ++i) {
			ConfusionMatrix* tmpCM = resultingConfusionMatrices[i];
			sum_accuracy += pow((tmpCM->getAccuracy()-mean_accuracy),2);
			sum_precision += pow((tmpCM->getPrecision()-mean_precision),2);
			sum_truePositive += pow((tmpCM->getTruePositive()-mean_truePositive),2);
		}
		
		
		float std_deviation_accuracy = sqrt(sum_accuracy / numberOfConfMatrices);
		float std_deviation_precision = sqrt(sum_precision / numberOfConfMatrices);
		float std_deviation_truePositive = sqrt(sum_truePositive / numberOfConfMatrices);

		cout << "TruePositive Mean\Accuracy: " << mean_accuracy << " \ " << std_deviation_accuracy << endl;
		cout << "Precision Mean\Precision: " << mean_precision << " \ " << std_deviation_precision << endl;
		cout << "TruePositive Mean\StdDeviation: " << mean_truePositive << " \ " << std_deviation_truePositive << endl;
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