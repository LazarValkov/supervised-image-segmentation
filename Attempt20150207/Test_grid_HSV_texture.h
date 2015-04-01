#include "Tester.h"
#include "Classifier.h"
#include "NeuralNetworkClassiffier.h"

class Test_grid_HSV_texture : public Tester {
public:

	Test_grid_HSV_texture(bool ttrainClassifier, bool lloadClassifierFromFile, GlobalVariables::Organ organ)
		: Tester(ttrainClassifier, lloadClassifierFromFile) {
		description = "Test_grid_HSV_Texture";
		classifierFilename = classifiersStoreFolder + "test_grid_HSV_texture.txt";
		classifierType = Classifier::ClassifierType::NeuralNetwork;
		IAFunctions = ImageAnalyser::IA_Functions::TO_GRID | ImageAnalyser::IA_Functions::ADD_HSV_BLURRED
			| ImageAnalyser::IA_Functions::ADD_LAWS_ENERGY_MEASURES | ImageAnalyser::IA_Functions::TEXTURE_WAVELET;
		featureVectorSize_Input = 24;
		featureVectorSize_Output = 1;
		positiveOrgan = organ;
	}
	
	int nn_HiddenUnits = 27;
	int nn_maxInterations = 2000;
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