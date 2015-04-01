#include "Tester.h"
#include "Classifier.h"
#include "NeuralNetworkClassiffier.h"

class Test_grid_LAB : public Tester {
public:

	Test_grid_LAB(bool ttrainClassifier, bool lloadClassifierFromFile, GlobalVariables::Organ organ)
		: Tester(ttrainClassifier, lloadClassifierFromFile) {
		description = "Test_grid_LAB";
		classifierFilename = classifiersStoreFolder + "test_grid_LAB.txt";
		classifierType = Classifier::ClassifierType::NeuralNetwork;
		IAFunctions = ImageAnalyser::IA_Functions::TO_GRID | ImageAnalyser::IA_Functions::ADD_LAB;
		featureVectorSize_Input = 3;
		featureVectorSize_Output = 1;
		positiveOrgan = organ;
	}

	int nn_HiddenUnits = 10;
	int nn_maxInterations = 10000;
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