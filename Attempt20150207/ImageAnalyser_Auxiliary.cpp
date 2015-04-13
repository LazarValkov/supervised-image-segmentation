#include "ImageAnalyser.h"
#include <fstream>
#include <iostream>
#include "DataVisualiser.h"

using namespace std;

void ImageAnalyser::prepareClassificationData(Mat &input_f, Mat &correctOutput_f,
	const IA_Functions functionsSelection
	,vector<ImageInfo*> imagesInfos, int imgWidth, int imgHeight, bool isTraining,
	const int featureVectorSize_input, const int featureVectorSize_output, GlobalVariables::Organ positiveOrgan, GlobalVariables::Organ negativeOrgans) {

	
	int numOfListedImages = imagesInfos.size();
	/*Create the input and desired output matrices*/
	Mat *input = new Mat(numOfListedImages*imgWidth*imgHeight, featureVectorSize_input, CV_32FC1);
	Mat *correctOutput = new Mat(numOfListedImages*imgWidth*imgHeight, featureVectorSize_output, CV_32FC1);

	/*Now populate them*/
	int entryCount = 0;
	for (int i = 0; i < numOfListedImages; ++i) {

		//cii - current image info
		ImageInfo* cii = imagesInfos[i];

		
		vector<int> organsCount;
		organsCount.push_back(cii->countOthers); organsCount.push_back(cii->countHeart);
		organsCount.push_back(cii->countLungs); organsCount.push_back(cii->countDiaphragm);
		organsCount.push_back(cii->countLiver);

		cout << "processing " << cii->filename_img << endl;
		processImage(cii->filename_img, cii->filename_aImg, isTraining,
			*input, *correctOutput, entryCount, functionsSelection,
			organsCount, positiveOrgan, negativeOrgans);
	}

	/**Now trim the matrices**/
	input_f = (*input)(Range(0, entryCount), Range(0, featureVectorSize_input));
	delete input;
	correctOutput_f = (*correctOutput)(Range(0, entryCount), Range(0, featureVectorSize_output));
	delete correctOutput;
}

void ImageAnalyser::processImage(string& filenameImg, string& filenameAnnotatedImg, bool isForTraining
	, Mat &input, Mat &correctOutput, int &currentEntry, const IA_Functions functionsSelection
	, vector<int> &organsCount, GlobalVariables::Organ positiveOrgan, GlobalVariables::Organ negativeOrgans) {

	Mat image = imread(filenameImg);
	Mat annotatedImage = imread(filenameAnnotatedImg);
	vector<Mat> inputFeatureMatrixVector;
	//startingEntry is used for visualisation only
	int startingEntry = currentEntry;

	if (functionsSelection & COLOR_HIST_EQ) {
		equaliseColorHisto(image);
	}
	if (functionsSelection & TO_DOWNSAMPLE_GPYRAMID) {
		downsampleUsingGPyramid(image, annotatedImage, organsCount);
	}
	if (functionsSelection & ADD_HSV) {
		addHsv(image, inputFeatureMatrixVector);
	}
	if (functionsSelection & ADD_HSV_BLURRED) {
		addHsvBlurred(image, inputFeatureMatrixVector);
	}
	if (functionsSelection & ADD_LAB) {
		addLab(image, inputFeatureMatrixVector);
	}
	if (functionsSelection & ADD_LAWS_ENERGY_MEASURES) {
		addLawsEnergyMeasures(image, inputFeatureMatrixVector);
	}
	if (functionsSelection & ADD_LAWS_ENERGY_MEASURES_GAUSSIAN_PYRAMID) {
		addLawsEnergyMeasuresGaussianPyramid(image, inputFeatureMatrixVector);
	}
	if (functionsSelection & ADD_LAWS_ENERGY_MEASURES_ON_HSV) {
		addLawsEnergyMeasuresOnHSV(image, inputFeatureMatrixVector);
	}
	if (functionsSelection & ADD_Y) {
		addY(image, inputFeatureMatrixVector);
	}
	if (functionsSelection & TEXTURE_WAVELET) {
		//addWaveletMeasures_myInitialIdea(filenameImg.substr(filenameImg.length() - 12), inputFeatureMatrixVector);
		addWaveletMeasures_unsupervisedTextureSegmentationPaper(filenameImg.substr(filenameImg.length() - 12), inputFeatureMatrixVector);
	}
	if (functionsSelection & TO_GRID) {
		convertToGrid(image, annotatedImage, organsCount, inputFeatureMatrixVector);
	}
	addImageToInput_Binary(image, annotatedImage, inputFeatureMatrixVector
		, input, correctOutput, organsCount, positiveOrgan, negativeOrgans, isForTraining, currentEntry);
	
	if (DataVisualiser::isOn)
		DataVisualiser::visualisingData.push_back(DataVisualiser::VisualisingData(annotatedImage, positiveOrgan, negativeOrgans, startingEntry));
}

void ImageAnalyser::addImageToInput_Binary_CalculateOrgansAddingChance(GlobalVariables::Organ positiveOrgan, GlobalVariables::Organ negativeOrgans, vector<int>& organsCount, vector<int>& organsAddingChance, bool isForTraining) {
	int posExamplesCount = organsCount[GlobalVariables::getOrganIndex(positiveOrgan)];
	int negExamplesCount = 0;
	int negOrgansCount = 0;

	int organsCountSize = organsCount.size();
	int lastOrganIntValue = pow(2, organsCountSize);
	
	/**get the sum of all the negative examples and calculate **/
	for (int i = GlobalVariables::OTHERS; i < lastOrganIntValue; i *= 2) {
		GlobalVariables::Organ currentOrgan = static_cast<GlobalVariables::Organ>(i);
		int currentOrganIndex = GlobalVariables::getOrganIndex(currentOrgan);
		if (currentOrgan & negativeOrgans) {
			++negOrgansCount;
			negExamplesCount += organsCount[currentOrganIndex];
		}
	}
	
	int negativeOrganAddingChance = 100;
	int positiveOrganAddingChance = 100;

	if (isForTraining) {
		if (negExamplesCount > posExamplesCount) {
			negativeOrganAddingChance = negExamplesCount == 0 ? 0 :
				(int)((posExamplesCount * 100) / negExamplesCount);
		}
		else if (negExamplesCount < posExamplesCount){
			positiveOrganAddingChance = posExamplesCount == 0 ? 0 :
				(int)((negExamplesCount * 100) / posExamplesCount);
		}
	}

	for (int i = GlobalVariables::OTHERS; i < lastOrganIntValue; i *= 2) {
		GlobalVariables::Organ currentOrgan = static_cast<GlobalVariables::Organ>(i);
		int currentOrganIndex = GlobalVariables::getOrganIndex(currentOrgan);
		if (currentOrgan & negativeOrgans)
			organsAddingChance.push_back(negativeOrganAddingChance);
		else if (currentOrgan == positiveOrgan)
			organsAddingChance.push_back(positiveOrganAddingChance);
		else
			organsAddingChance.push_back(0);
	}

	//since the count of organs, other than the positiveOrgan is 4
	
	//int examplesPerOrgan = posExamples / 4;

	/*for (int i = 0; i < organsCountSize; ++i)
	{
		int currentOrgansAddingChance = 0;
		if (isForTraining) {
			int currentOrganExamplesCount = organsCount[i];
			if (currentOrganExamplesCount < examplesPerOrgan || i == positiveOrgan)
				currentOrgansAddingChance = 100;
			else
				currentOrgansAddingChance = (int)(examplesPerOrgan * 100) / ((float)currentOrganExamplesCount);
		}
		else {
			currentOrgansAddingChance = 100;
		}
		
		organsAddingChance.push_back(currentOrgansAddingChance);
	}*/
	
	/*DEBUG: add only heart, lungs to see if results can be improved
	//others
	organsAddingChance.push_back(0);
	//heart
	organsAddingChance.push_back(100);
	//lungs
	int lungsAddingChance = isForTraining ? (int)(posExamples * 100) / organsCount[2] : 100;
	organsAddingChance.push_back(lungsAddingChance);
	//diaphragm
	organsAddingChance.push_back(0);
	//liver
	organsAddingChance.push_back(0);
	*/

	/*DEBUG: add only lungs vs HEART+OTHERS, rest others 
	int heartAndOthersAddingChance = 100;
	int lungsAddingChance = 100;
	int heartAndOthersCount = organsCount[0] + organsCount[1];
	int lungsCount = organsCount[2];

	if (isForTraining) {
		int heartAndOthersCount = organsCount[0] + organsCount[1];
		if (heartAndOthersCount > lungsCount) {
			heartAndOthersAddingChance = heartAndOthersCount == 0 ? 0 :
				(int)((lungsCount * 100) / heartAndOthersCount);
		}
		else if (heartAndOthersCount < lungsCount){
			lungsAddingChance = heartAndOthersCount == 0 ? 0 :
				(int)((heartAndOthersCount * 100) / lungsCount);
		}
	}
	
	
	//others
	organsAddingChance.push_back(heartAndOthersAddingChance);
	//heart

	organsAddingChance.push_back(heartAndOthersAddingChance);
	//lungs
	organsAddingChance.push_back(lungsAddingChance);
	//diaphragm
	organsAddingChance.push_back(0);
	//liver
	organsAddingChance.push_back(0);
	*/

}

void ImageAnalyser::addImageToInput_Binary(Mat &image, Mat& annotatedImage, vector<Mat>& inputFeatureMatrixVector
	, Mat& input, Mat& correctOutput, vector<int> &organsCount, GlobalVariables::Organ positiveOrgan, GlobalVariables::Organ negativeOrgans, bool isForTraining, int &currentEntry) {

	/**Add acquired information to the input/output matrices**/
	int rowsCount = image.rows;
	int colsCount = image.cols;

	vector<int> organsAddingChance;
	addImageToInput_Binary_CalculateOrgansAddingChance(positiveOrgan, negativeOrgans, organsCount, organsAddingChance, isForTraining);	

	GlobalVariables::Organ currentOrgan;
	int currentOrganExampleAddingChance;
	

	int j, i;
	for (j = rowsCount - 1; j > -1; j--)
	{
		for (i = colsCount - 1; i > -1; i--)
		{
			Vec3b annotatedImgIntensity = annotatedImage.at<Vec3b>(j, i);
			/**Add only offal pixels**/
			if (annotatedImgIntensity == GlobalVariables::COLOR_OFFAL_BACKGROUND)
				continue;

			currentOrgan = GlobalVariables::getOrganByColor(annotatedImgIntensity);
			currentOrganExampleAddingChance = organsAddingChance[GlobalVariables::getOrganIndex(currentOrgan)];

			/**Check what the class is and make a decision whether to add it**/
			float outputClass = currentOrgan == positiveOrgan ? 1 : 0;

			//else if (!isForTr9aining) {/*empty so that the code execution can continue;*/ }
			if ((rand() % 100) + 1 > currentOrganExampleAddingChance) continue;
			
			
			/**Set expected output**/
			correctOutput.at<float>(currentEntry, 0) = outputClass;

			for (int currentMatInd = 0; currentMatInd < inputFeatureMatrixVector.size(); ++currentMatInd) {
				input.at<float>(currentEntry, currentMatInd)
					= inputFeatureMatrixVector[currentMatInd].at<float>(j, i);
			}

			++currentEntry;

			/*DEBUG
			Vec3b imgDebugColor_positive = Vec3b(7, 135, 255);
			Vec3b imgDebugColor_rest = Vec3b(236, 43, 197);
			if (currentOrgan == positiveOrgan)
				annotatedImage.at<Vec3b>(j, i) = imgDebugColor_positive;
			else
				annotatedImage.at<Vec3b>(j, i) = imgDebugColor_rest;*/
		}
	}
}


void ImageAnalyser::shuffleMatrixRows(Mat& matrix1, Mat& matrix2) {
	std::vector<int> myvector;
	int rowsCount = matrix1.rows;
	int cols1Count = matrix1.cols;

	int cols2Count = matrix2.cols;
	//DEBUG: PRE-SHUFFLE OUTPUT
	//cout << matrix.at<float>(4, 0) << '\t' << matrix.at<float>(4, 1) << '\t' << matrix.at<float>(4, 2) << '\t' << matrix.at<float>(4, 3) << endl;

	//set index values
	for (int i = 0; i < rowsCount; ++i) {
		myvector.push_back(i); // 0 to rowsCount-1
	}
	// using built-in random generator:
	std::random_shuffle(myvector.begin(), myvector.end());
	// DEBUG: make sure the permutations are the same

	int currentRow = 0;
	int swapWithIndex = 0;
	float helpingSwappingVariable;
	for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it, ++currentRow) {
		swapWithIndex = *it;
		for (int i = 0; i < cols1Count; i++) {
			helpingSwappingVariable = matrix1.at<float>(swapWithIndex, i);
			matrix1.at<float>(swapWithIndex, i) = matrix1.at<float>(currentRow, i);
			matrix1.at<float>(currentRow, i) = helpingSwappingVariable;
		}
		for (int i = 0; i < cols2Count; i++) {
			helpingSwappingVariable = matrix2.at<float>(swapWithIndex, i);
			matrix2.at<float>(swapWithIndex, i) = matrix2.at<float>(currentRow, i);
			matrix2.at<float>(currentRow, i) = helpingSwappingVariable;
		}
	}
	//DEBUG: PRE-SHUFFLE OUTPUT
	//cout << matrix.at<float>(4, 0) << '\t' << matrix.at<float>(4, 1) << '\t' << matrix.at<float>(4, 2) << '\t' << matrix.at<float>(4, 3) << endl;

}



float** ImageAnalyser::calculateNormalizationParameters(Mat &input_f) {
	int cols = input_f.cols;
	int rows = input_f.rows;
	float **normalisationParameters = new float*[cols];
	for (int i = 0; i < cols; ++i) {
		normalisationParameters[i] = new float[normalisationParametersCount];
		for (int i1 = 0; i1 < normalisationParametersCount; ++i1)
			normalisationParameters[i][i1] = 0;
	}
	
	/**First, calculate sum to get the mean**/
	for (int j = 0; j < rows; ++j) {
		for (int i = 0; i < cols; ++i) {
			normalisationParameters[i][attributeIndex_sum] += input_f.at<float>(j, i);
		}
	}
	//Calculate the mean
	for (int i = 0; i < cols; ++i)
		normalisationParameters[i][attributeIndex_mean] = normalisationParameters[i][attributeIndex_sum] / rows;
	
	//calculate the variance
	for (int j = 0; j < rows; ++j) {
		for (int i = 0; i < cols; ++i) {
			normalisationParameters[i][attributeIndex_varianceSum] 
				+= pow((input_f.at<float>(j, i) - normalisationParameters[i][attributeIndex_mean]),2);
		}
	}

	//Calculate the standardDeviation
	for (int i = 0; i < cols; ++i)
		normalisationParameters[i][attributeIndex_stdDeviation] 
			= sqrt(normalisationParameters[i][attributeIndex_varianceSum] / rows);


	return normalisationParameters;
}

void ImageAnalyser::normaliseData(Mat &input_f, float** normalisationParameters) {
	int cols = input_f.cols;
	int rows = input_f.rows;

	for (int j = 0; j < rows; ++j) {
		for (int i = 0; i < cols; ++i) {
			input_f.at<float>(j, i)
				= (input_f.at<float>(j, i) - normalisationParameters[i][attributeIndex_mean]) / normalisationParameters[i][attributeIndex_stdDeviation];
		}
	}
}