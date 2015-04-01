#include "Tester.h"
#include "Classifier.h"
#include "NeuralNetworkClassiffier.h"

class Test_grid_laws : public Tester {
public:

	Test_grid_laws(bool ttrainClassifier, bool lloadClassifierFromFile, GlobalVariables::Organ organ)
		: Tester(ttrainClassifier, lloadClassifierFromFile) {
		description = "Test_grid_laws_texture";
		classifierFilename = classifiersStoreFolder + "Test_grid_laws.txt";
		classifierType = Classifier::ClassifierType::NeuralNetwork;
		IAFunctions = ImageAnalyser::IA_Functions::ADD_LAWS_ENERGY_MEASURES | ImageAnalyser::IA_Functions::TO_GRID;
		featureVectorSize_Input = 9;
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