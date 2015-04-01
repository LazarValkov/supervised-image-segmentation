#include "Tester.h"
#include "Classifier.h"
#include "BinaryAdaBoostClassifier.h"

class Test_grid_HSV_AB : public Tester {
public:

	Test_grid_HSV_AB(bool ttrainClassifier, bool lloadClassifierFromFile)
		: Tester(ttrainClassifier, lloadClassifierFromFile) {
		description = "Test_grid_HSV_AB";
		classifierFilename = classifiersStoreFolder + "test_grid_HSV_AB.txt";
		classifierType = Classifier::ClassifierType::NeuralNetwork;
		IAFunctions = ImageAnalyser::IA_Functions::TO_GRID | ImageAnalyser::IA_Functions::ADD_HSV;
		featureVectorSize_Input = 3;
		featureVectorSize_Output = 1;
		positiveOrgan = GlobalVariables::Organ::LIVER;
	}


	virtual Classifier* getClassifier() {
		Classifier *c;
		if (loadClassifierFromFile)
			c = new BinaryAdaBoostClassifier(classifierFilename);
		else {
			BinaryAdaBoostClassifier *nnc = new BinaryAdaBoostClassifier();
			c = nnc;
		}
		return c;
	}
};