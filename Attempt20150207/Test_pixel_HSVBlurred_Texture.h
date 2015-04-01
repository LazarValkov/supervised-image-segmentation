#include "Tester.h"
#include "Classifier.h"
#include "NeuralNetworkClassiffier.h"

class Test_pixel_HSVBlurred_Texture : public Tester {
public:

	Test_pixel_HSVBlurred_Texture(bool ttrainClassifier, bool lloadClassifierFromFile, GlobalVariables::Organ organ)
		: Tester(ttrainClassifier, lloadClassifierFromFile) {
		description = "Test_pixel_HSVBlurred_Texture";
		classifierFilename = classifiersStoreFolder + "Test_pixel_HSVBlurred_Texture.txt";
		classifierType = Classifier::ClassifierType::NeuralNetwork;
		IAFunctions =
			ImageAnalyser::IA_Functions::ADD_HSV_BLURRED | ImageAnalyser::IA_Functions::ADD_LAWS_ENERGY_MEASURES;
		featureVectorSize_Input = 12;
		featureVectorSize_Output = 1;
		positiveOrgan = organ;
	}

	int nn_HiddenUnits = 15;
	int nn_maxInterations = 500;
	virtual Classifier* getClassifier() {
		Classifier *c;
		if (loadClassifierFromFile)
			c = new NeuralNetworkClassifier(classifierFilename);
		else {
			NeuralNetworkClassifier *nnc = new NeuralNetworkClassifier(featureVectorSize_Input, nn_HiddenUnits, featureVectorSize_Output);
			nnc->max_iterations = nn_maxInterations;
			c = nnc;
		}
		return c;
	}
};