#ifndef IMAGE_ANALYSER_H
#define IMAGE_ANALYSER_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include "GlobalVariables.h"
#include "ImageInfo.h"

using namespace cv;

class ImageAnalyser {
public:
	static enum IA_Functions {
		TO_GRID = 1, ADD_HSV = 2, ADD_LAB = 4,
		ADD_LAWS_ENERGY_MEASURES = 8, ADD_LAWS_ENERGY_MEASURES_GAUSSIAN_PYRAMID = 16, ADD_HSV_BLURRED = 32,
		ADD_LAWS_ENERGY_MEASURES_ON_HSV = 64, TO_DOWNSAMPLE_GPYRAMID = 128, ADD_Y = 256, COLOR_HIST_EQ = 512,
		TEXTURE_WAVELET = 1024
	};

	static void prepareClassificationData(Mat &input_f, Mat &correctOutput_f,
		const IA_Functions functionsSelection
		, vector<ImageInfo*> imagesInfos, int imgWidth, int imgHeight, bool isTraining,
		const int featureVectorSize_input, const int featureVectorSize_output, GlobalVariables::Organ positiveOrgan);
	static void processImage(string& filenameImg, string& filenameAnnotatedImg, bool isForTraining
		, Mat &input, Mat &correctOutput, int &currentEntry, const IA_Functions functionsSelection
		, vector<int> &organsCount, GlobalVariables::Organ positiveOrgan);
	static void addImageToInput_Binary(Mat &image, Mat& annotatedImage
		, vector<Mat>& inputFeatureMatrixVector, Mat& input, Mat& correctOutput
		, vector<int> &organsCount, GlobalVariables::Organ positiveOrgan
		, bool isForTraining, int &currentEntry);
	static void addImageToInput_Binary_CalculateOrgansAddingChance(GlobalVariables::Organ positiveOrgan, vector<int>& organsCount, vector<int>& organsAddingChance, bool isForTraining);
	static void convertToGrid(Mat &image, Mat &annotatedImage, vector<int> &organsCount, vector<Mat>& inputFeatureMatrixVector);
	static void downsampleUsingGPyramid(Mat &image, Mat &annotatedImage, vector<int> &organsCount);
	
	/***Colors***/
	static void addHsvBlurred(Mat &img, vector<Mat>& inputFeatureMatrixVector);
	static void addHsv(Mat &img, vector<Mat>& inputFeatureMatrixVector);
	static void addLab(Mat &img, vector<Mat>& inputFeatureMatrixVector);
	
	/***Texture***/
	
	static void addLawsEnergyMeasures(Mat &img, vector<Mat>& inputFeatureMatrixVector);
	static void addLawsEnergyMeasuresOnHSV(Mat &img, vector<Mat>& inputFeatureMatrixVector);
	static void addLawsEnergyMeasuresGaussianPyramid(Mat &img, vector<Mat>& inputFeatureMatrixVector);
	static void extractLaws(Mat& oneChannelFloatImg, vector<Mat>& ms_tex_ene_fin);
	
	static void addWaveletMeasures(string& waveletFilename, vector<Mat>& inputFeatureMatrixVector);

	/***Other***/
	static void addY(Mat &img, vector<Mat>& inputFeatureMatrixVector);
	static void equaliseColorHisto(Mat &img);

	static void shuffleMatrixRows(Mat& matrix1, Mat& matrix2);
	
	/***************Normalisation-related***********************/
	// need 4 - Sum, Mean, Variance, Standard Diviation
	static const int attributeIndex_sum = 0;
	static const int attributeIndex_varianceSum = 1;
	static const int attributeIndex_mean = 2;
	static const int attributeIndex_stdDeviation = 3;
	static const int normalisationParametersCount = 4;

	static float** calculateNormalizationParameters(Mat &input_f);
	static void normaliseData(Mat &input_f, float** normalizationParameters);
	/***********************************************************/


	static friend inline IA_Functions  operator|(IA_Functions a, IA_Functions b)
	{
		return static_cast<IA_Functions>(static_cast<int>(a) | static_cast<int>(b));
		//return static_cast<IA_Functions>(a | b);
	}
	static string lastProcessedImgFilename, lastProcessedImgAnnotatedFilename;

	/**Public parameters to be modified**/
	//TO_GRID
	static int gridSize;
	static int euclideanSize;

	static int AddHsvBlurred_medianBlurKernalSize;
	
};

#endif