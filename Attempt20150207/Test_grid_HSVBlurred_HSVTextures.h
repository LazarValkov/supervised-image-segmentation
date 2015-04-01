#include "Tester.h"
#include "Classifier.h"
#include "NeuralNetworkClassiffier.h"

class Test_grid_HSVBlurred_HSVTextures : public Tester {
public:

	Test_grid_HSVBlurred_HSVTextures(bool ttrainClassifier, bool lloadClassifierFromFile, GlobalVariables::Organ organ)
		: Tester(ttrainClassifier, lloadClassifierFromFile) {
		description = "Test_grid_HSVBlurred_HSVTextures";
		classifierFilename = classifiersStoreFolder + "Test_grid_HSVBlurred_HSVTextures_Diaphragm.txt";
		classifierType = Classifier::ClassifierType::NeuralNetwork;
		IAFunctions = ImageAnalyser::IA_Functions::TO_GRID | ImageAnalyser::IA_Functions::ADD_HSV_BLURRED
			| ImageAnalyser::IA_Functions::ADD_LAWS_ENERGY_MEASURES_ON_HSV;
		featureVectorSize_Input = 30;
		featureVectorSize_Output = 1;
		positiveOrgan = organ;

		ImageAnalyser::gridSize = 5;
		ImageAnalyser::AddHsvBlurred_medianBlurKernalSize = 3;
	}

	int nn_HiddenUnits = 35;
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