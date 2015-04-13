#include "Tester.h"
#include "Classifier.h"
#include "NeuralNetworkClassiffier.h"

class Test_grid_wavelets : public Tester {
public:

	Test_grid_wavelets(bool ttrainClassifier, bool lloadClassifierFromFile, GlobalVariables::Organ posOrgan, GlobalVariables::Organ negOrgans)
		: Tester(ttrainClassifier, lloadClassifierFromFile) {
		description = "Test_grid_wavelets_texture";
		classifierFilename = classifiersStoreFolder + "Test_grid_wavelets.txt";
		classifierType = Classifier::ClassifierType::NeuralNetwork;
		IAFunctions = ImageAnalyser::IA_Functions::TEXTURE_WAVELET | ImageAnalyser::IA_Functions::TO_GRID;
		featureVectorSize_Input = 4;
		featureVectorSize_Output = 1;
		positiveOrgan = posOrgan;
		negativeOrgans = negOrgans;		
	}

	int nn_HiddenUnits = 7;
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