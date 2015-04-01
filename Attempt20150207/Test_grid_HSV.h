#include "Tester.h"
#include "Classifier.h"
#include "NeuralNetworkClassiffier.h"
#include "ImageAnalyser.h"

class Test_grid_HSV : public Tester {
public:
	
	Test_grid_HSV(bool ttrainClassifier, bool lloadClassifierFromFile, GlobalVariables::Organ organ)
		: Tester(ttrainClassifier, lloadClassifierFromFile) {
		description = "Test_grid_HSV";
		classifierFilename = classifiersStoreFolder + "test_grid_HSV.txt";
		classifierType = Classifier::ClassifierType::NeuralNetwork;
		IAFunctions = ImageAnalyser::IA_Functions::TO_GRID | ImageAnalyser::IA_Functions::ADD_HSV_BLURRED;// | ImageAnalyser::IA_Functions::TEXTURE_WAVELET;
		featureVectorSize_Input = 3;
		featureVectorSize_Output = 1;
		positiveOrgan = organ;
	}

	int nn_HiddenUnits = 5;
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