#ifndef BINARY_CONFUSION_MATRIX
#define BINARY_CONFUSION_MATRIX
#include "ConfusionMatrix.h"
#define NUMBER_OF_CLASSES 2
class BinaryConfusionMatrix : public ConfusionMatrix {
public:
	BinaryConfusionMatrix();
	void logPrediction(int expectedClass, int predictedClass);
	void calculateOverallMeasures();
	//friend ostream& operator<< (ostream& outStream, BinaryConfusionMatrix confusionMatrix);
	virtual std::ostream& outputString(std::ostream& o);

	virtual float getAccuracy() { return accuracy; }
	virtual float getPrecision() { return precision; }
	virtual float getTruePositive() { return  truePositive; }



private:
	float confusionMatrix[NUMBER_OF_CLASSES + 2][NUMBER_OF_CLASSES + 3];
	float accuracy;
	float precision;
	float truePositive;
	float fmeasure;
};
//ostream& operator<< (ostream& outStream, BinaryConfusionMatrix confusionMatrix);

#endif