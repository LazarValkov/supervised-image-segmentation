#include "ConfusionMatrix.h"

ConfusionMatrix::ConfusionMatrix(int aNumOfClasses) : numberOfClasses(aNumOfClasses) {

	for (int j = 0; j < NUM_ORGANS + 1; ++j) {
		for (int i = 0; i < NUM_ORGANS + 3; ++i) {
			confusionMatrix[j][i] = 0;
		}
	}
}

void ConfusionMatrix::logPrediction(int expectedClass, int predictedClass) {
	confusionMatrix[expectedClass][predictedClass]++;

}

void ConfusionMatrix::calculateOverallMeasures(){

	float totalItemsCount = 0.0f;

	//Calculate for each class:
	for (int currentClass = 0; currentClass < NUM_ORGANS; currentClass++) {

		//find 'd'
		float d = (float)confusionMatrix[currentClass][currentClass];
		//find 'c'
		// iterate through the ith row (skipping the ith column) and sum
		float c = 0.0f;
		for (int t = 0; t < NUM_ORGANS; t++) {
			if (t == currentClass)
				continue;
			c += confusionMatrix[currentClass][t];
		}
		//find 'b'
		// iterate through the ith column (skipping the ith row) and sum
		float b = 0.0f;
		for (int t = 0; t < NUM_ORGANS; t++) {
			if (t == currentClass)
				continue;
			b += confusionMatrix[t][currentClass];
		}
		//find 'a'
		//go through the matrix (skipping the ith row and ith column) and sum
		float a = 0.0f;
		for (int j1 = 0; j1 < NUM_ORGANS; j1++) {
			if (j1 == currentClass)
				continue;
			for (int i1 = 0; i1 < NUM_ORGANS; i1++) {
				if (i1 == currentClass)
					continue;
				a += confusionMatrix[j1][i1];
			}
		}
		/*
		//calculate the recall
		cout << "---"<< currentClass << "---" << endl;
		cout << "a=" << a << endl;
		cout << "b=" << b << endl;
		cout << "c=" << c << endl;
		cout << "d=" << d << endl;
		*/

		float recall = d / (c + d);
		confusionMatrix[currentClass][NUM_ORGANS] = recall * 100;
		float precision = d / (b + d);
		confusionMatrix[NUM_ORGANS][currentClass] = precision * 100;
		float f_measure = (2 * precision*recall) / (precision + recall);
		confusionMatrix[currentClass][NUM_ORGANS + 1] = f_measure;

		//firstly, sum all the items for this class
		confusionMatrix[currentClass][NUM_ORGANS + 2] = 0;
		for (int i = 0; i < NUM_ORGANS; i++) {
			confusionMatrix[currentClass][NUM_ORGANS + 2] += confusionMatrix[currentClass][i];
		}
		totalItemsCount += confusionMatrix[currentClass][NUM_ORGANS + 2];
	}

	// calculate item percentats for each class
	for (int currentClass = 0; currentClass < NUM_ORGANS; ++currentClass) {
		confusionMatrix[currentClass][NUM_ORGANS + 2] = (confusionMatrix[currentClass][NUM_ORGANS + 2] / totalItemsCount) * 100;
	}
}


std::ostream& ConfusionMatrix::outputString(std::ostream& outStream) {
	for (int j = 0; j < NUM_ORGANS; j++) {
		for (int i = 0; i < NUM_ORGANS + 3; i++) {
			outStream << confusionMatrix[j][i] << '\t';
		}
		outStream << endl;
	}
	outStream << "----" << endl;
	cout.precision(2);
	outStream << fixed;
	for (int i = 0; i < NUM_ORGANS; i++) {
		outStream << confusionMatrix[5][i] << '\t';
	}
	outStream << endl;
	return outStream;
}

ostream& operator<< (ostream& outStream, ConfusionMatrix& confusionMatrix) {	
	return confusionMatrix.outputString(outStream);
}