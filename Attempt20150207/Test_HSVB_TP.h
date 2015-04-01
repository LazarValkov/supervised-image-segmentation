#include "Tester.h"
#include "Classifier.h"
#include "NeuralNetworkClassiffier.h"

class Test_pixel_HSVBlurred_TP : public Tester {
public:

	Test_pixel_HSVBlurred_TP(bool ttrainClassifier, bool lloadClassifierFromFile, GlobalVariables::Organ organ)
		: Tester(ttrainClassifier, lloadClassifierFromFile) {
		description = "Test_pixel_HSVBlurred_TP";
		classifierFilename = classifiersStoreFolder + "Test_pixel_HSVBlurred_TP.txt";
		classifierType = Classifier::ClassifierType::NeuralNetwork;
		IAFunctions =
			ImageAnalyser::TO_DOWNSAMPLE_GPYRAMID | ImageAnalyser::ADD_Y | ImageAnalyser::COLOR_HIST_EQ |
			ImageAnalyser::IA_Functions::ADD_HSV_BLURRED | ImageAnalyser::IA_Functions::ADD_LAWS_ENERGY_MEASURES;
		featureVectorSize_Input = 13;
		featureVectorSize_Output = 1;
		positiveOrgan = organ;
	}

	int nn_HiddenUnits = 16;
	int nn_maxInterations = 350;
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