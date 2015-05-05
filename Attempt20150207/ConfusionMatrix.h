#ifndef CONFUSION_MATRIX_H
#define CONFUSION_MATRIX_H
#include <iostream>
#define NUM_ORGANS 5
using namespace std;

class ConfusionMatrix {
public:
	ConfusionMatrix(int aNumOfClasses);
	virtual void logPrediction(int expectedClass, int predictedClass);
	virtual void calculateOverallMeasures();
	virtual std::ostream& outputString(std::ostream& o);

	virtual float getAccuracy() { return -1; }
	virtual float getPrecision() { return -1; }
	virtual float getTruePositive() { return -1; }
	virtual float getFMeasure() { return -1; }

	
protected:
	const int numberOfClasses;
private:
	int confusionMatrix[NUM_ORGANS + 1][NUM_ORGANS + 3];
	//friend ostream& operator<< (ostream& outStream, ConfusionMatrix confusionMatrix);
};
ostream& operator<< (ostream& outStream, ConfusionMatrix& confusionMatrix);

#endif