#include "Tester.h"
#include "Classifier.h"
#include "NeuralNetworkClassiffier.h"

class Test_grid_colorBlurred : public Tester {
public:
	Test_grid_colorBlurred(bool ttrainClassifier, bool lloadClassifierFromFile, GlobalVariables::Organ posOrgan, GlobalVariables::Organ negOrgans)
		: Tester(ttrainClassifier, lloadClassifierFromFile) {
		description = "Test_grid_colorBlurred_NeuralNetwork";
		classifierFilename = classifiersStoreFolder + "test_grid_colorBlurred.txt";
		classifierType = Classifier::ClassifierType::NeuralNetwork;
		IAFunctions = ImageAnalyser::IA_Functions::ADD_HSV_BLURRED | ImageAnalyser::IA_Functions::TO_GRID;// | ImageAnalyser::IA_Functions::COLOR_HIST_EQ;
		featureVectorSize_Input = 3;
		featureVectorSize_Output = 1;
		positiveOrgan = posOrgan;
		negativeOrgans = negOrgans;
	}

	int nn_HiddenUnits = 6;
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