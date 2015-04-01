#include "BinaryConfusionMatrix.h"

BinaryConfusionMatrix::BinaryConfusionMatrix() : ConfusionMatrix(2) {
	for (int j = 0; j < NUMBER_OF_CLASSES + 2; ++j) {
		for (int i = 0; i < NUMBER_OF_CLASSES + 3; ++i) {
			confusionMatrix[j][i] = 0;
		}
	}
}
//testcomment
void BinaryConfusionMatrix::logPrediction(int expectedClass, int predictedClass) {
	confusionMatrix[expectedClass][predictedClass]++;
}
void BinaryConfusionMatrix::calculateOverallMeasures() {
	float a = confusionMatrix[0][0];
	float b = confusionMatrix[0][1];
	float c = confusionMatrix[1][0];
	float d = confusionMatrix[1][1];

	accuracy = (a + d) / (a + b + c + d);
	precision = d / (b + d);
	truePositive = d / (c + d);

	float beta = 1;
	float betaSqaured = beta*beta;
	fmeasure = (1 + betaSqaured)*(precision*truePositive) / (betaSqaured*precision + truePositive);
}

std::ostream& BinaryConfusionMatrix::outputString(std::ostream& outStream) {
	int a00 = confusionMatrix[0][0];
	int a01 = confusionMatrix[0][1];
	int a10 = confusionMatrix[1][0];
	int a11 = confusionMatrix[1][1];

	outStream << "\t\t\t" << "   Predicted:" << endl;
	outStream << "\t\t\t0\t1\t" << endl;
	outStream << "Expected:\t0:\t" << a00 << '\t' << a01 << endl;
	outStream << "\t\t1:\t" << a10 << '\t' << a11 << endl;

	outStream << "_______________________________________" << endl;

	outStream << "Accuracy: \t\t" << accuracy << endl;
	outStream << "Precision: \t\t" << precision << endl;
	outStream << "truePositive(Recall)\t: " << truePositive << endl;
	outStream << "fmeasure: \t\t" << fmeasure << endl;

	outStream << endl;
	return outStream;
}

//ostream& operator<< (ostream& outStream, BinaryConfusionMatrix confusionMatrix) {}