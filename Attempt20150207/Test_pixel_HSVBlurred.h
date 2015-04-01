#include "Tester.h"
#include "Classifier.h"
#include "NeuralNetworkClassiffier.h"

class Test_pixel_HSVBlurred : public Tester {
public:
	Test_pixel_HSVBlurred(bool ttrainClassifier, bool lloadClassifierFromFile, GlobalVariables::Organ organ)
		: Tester(ttrainClassifier, lloadClassifierFromFile) {
		description = "Test_pixel_HSV_Blurred_NeuralNetwork";
		classifierFilename = classifiersStoreFolder + "test_pixel_HSVBlurred.txt";
		classifierType = Classifier::ClassifierType::NeuralNetwork;
		IAFunctions = ImageAnalyser::IA_Functions::ADD_HSV_BLURRED;
		featureVectorSize_Input = 3;
		featureVectorSize_Output = 1;
		positiveOrgan = organ;
	}
	int nn_HiddenUnits = 4;
	int nn_maxInterations = 500;
	virtual Classifier* getClassifier() {
		Classifier *c;
		if (loadClassifierFromFile)
			c = new NeuralNetworkClassifier(classifierFilename);
		else
			c = new NeuralNetworkClassifier(featureVectorSize_Input, nn_HiddenUnits, featureVectorSize_Output);
		return c;
	}
};