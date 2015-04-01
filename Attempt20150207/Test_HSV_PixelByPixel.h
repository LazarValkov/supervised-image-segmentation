#include "Tester.h"
#include "Classifier.h"
#include "NeuralNetworkClassiffier.h"

class Test_HSV_PixelByPixel : public Tester {
public:
	Test_HSV_PixelByPixel(bool ttrainClassifier, bool lloadClassifierFromFile) 
		: Tester(ttrainClassifier, lloadClassifierFromFile) {
		description = "TestHSV_PixelByPixel_NeuralNetwork";
		classifierFilename = classifiersStoreFolder + "test_HSV_PixelByPixel.txt";
		classifierType = Classifier::ClassifierType::NeuralNetwork;
		IAFunctions = ImageAnalyser::IA_Functions::ADD_HSV;
		featureVectorSize_Input = 3;
		featureVectorSize_Output = 1;
		positiveOrgan = GlobalVariables::Organ::LIVER;
	}
	int nn_HiddenUnits = 4;
	virtual Classifier* getClassifier() {
		Classifier *c;
		if (loadClassifierFromFile)
			c = new NeuralNetworkClassifier(classifierFilename);
		else
			c = new NeuralNetworkClassifier(featureVectorSize_Input, nn_HiddenUnits, featureVectorSize_Output);
		return c;
	}
};