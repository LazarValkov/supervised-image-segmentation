#include "ImageAnalyser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <complex>
#include <cmath>
#include <algorithm>
#include "wavelet2d.h"

void computeMeasures1(Mat &waveletImg, Mat &waveletImg_mean, Mat &waveletImg_variance, Mat &waveletImg_std, int row_from, int row_to, int col_from, int col_to, int winSize);
void computeMeasures2(Mat &waveletImg, Mat &energyMat, int row_from, int row_to, int col_from, int col_to, int winSize);
void addWaveletFeature(Mat &waveletFeature, int row_from, int row_to, int col_from, int col_to, int scale, vector<Mat>& inputFeatureMatrixVector);


const string waveletDir = "C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\wavelets_3\\db3\\";
// top-left - LL, bot-left - LH, top-right - HL, bot-right HH
//http://bit.kuas.edu.tw/~jihmsp/2011/vol2/JIH-MSP-2011-02-003.pdf
//getting mean/variance/std
void ImageAnalyser::addWaveletMeasures_myInitialIdea(string& waveletFilename, vector<Mat>& inputFeatureMatrixVector) {
	const int waveletLevels = 3;
	const int windowSize = 5;

	Mat waveletImgInt = imread(waveletDir + waveletFilename, 0);

	/************Compute the measures****************/
	Mat waveletImgFloat;
	waveletImgInt.convertTo(waveletImgFloat, CV_32F);
	Mat waveletImg_mean, waveletImg_variance, waveletImg_std;
	waveletImg_mean.create(waveletImgFloat.size(), waveletImgFloat.type());
	waveletImg_variance.create(waveletImgFloat.size(), waveletImgFloat.type());
	waveletImg_std.create(waveletImgFloat.size(), waveletImgFloat.type());
	//LL3
	int col_from = 0, col_to = 100, row_from = 0, row_to = 200, winSize = 10, scale = 8;
	computeMeasures1(waveletImgFloat, waveletImg_mean, waveletImg_variance, waveletImg_std, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_mean, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_variance, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_std, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	//HL3
	col_from = 100; col_to = 200; row_from = 0; row_to = 200; 
	computeMeasures1(waveletImgFloat, waveletImg_mean, waveletImg_variance, waveletImg_std, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_mean, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_variance, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_std, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	//LH3
	col_from = 0; col_to = 100; row_from = 200; row_to = 400;
	computeMeasures1(waveletImgFloat, waveletImg_mean, waveletImg_variance, waveletImg_std, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_mean, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_variance, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_std, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	//HH3
	col_from = 100; col_to = 200; row_from = 200; row_to = 400;
	computeMeasures1(waveletImgFloat, waveletImg_mean, waveletImg_variance, waveletImg_std, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_mean, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_variance, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_std, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	/*
	//LH2
	col_from = 0; col_to = 400; row_from = 400; row_to = 800; winSize *= 2; scale /= 2;
	computeMeasures1(waveletImgFloat, waveletImg_mean, waveletImg_variance, waveletImg_std, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_mean, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_variance, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_std, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);

	//HL2
	col_from = 200; col_to = 400; row_from = 0; row_to = 400;
	computeMeasures1(waveletImgFloat, waveletImg_mean, waveletImg_variance, waveletImg_std, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_mean, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	//addWaveletFeature(waveletImg_variance, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_std, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);

	//HH2
	col_from = 200; col_to = 400; row_from = 400; row_to = 800;
	computeMeasures1(waveletImgFloat, waveletImg_mean, waveletImg_variance, waveletImg_std, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_mean, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_variance, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_std, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	*/
}

void ImageAnalyser::addWaveletMeasures_unsupervisedTextureSegmentationPaper(string& waveletFilename, vector<Mat>& inputFeatureMatrixVector) {
	const int waveletLevels = 3;
	const int windowSize = 5;

	Mat waveletImgInt = imread(waveletDir + waveletFilename, 0);

	/************Compute the measures****************/
	Mat waveletImgFloat;
	waveletImgInt.convertTo(waveletImgFloat, CV_32F);
	Mat waveletImg_energy;
	waveletImg_energy.create(waveletImgFloat.size(), waveletImgFloat.type());
	
	//LL3
	int col_from = 0, col_to = 100, row_from = 0, row_to = 200, winSize = 10, scale = 8;
	computeMeasures2(waveletImgFloat, waveletImg_energy, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_energy, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);

	//HL3
	col_from = 100; col_to = 200; row_from = 0; row_to = 200;
	computeMeasures2(waveletImgFloat, waveletImg_energy, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_energy, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	//LH3
	col_from = 0; col_to = 100; row_from = 200; row_to = 400;
	computeMeasures2(waveletImgFloat, waveletImg_energy, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_energy, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	//HH3
	col_from = 100; col_to = 200; row_from = 200; row_to = 400;
	computeMeasures2(waveletImgFloat, waveletImg_energy, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_energy, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	/*
	//LH2
	col_from = 0; col_to = 400; row_from = 400; row_to = 800; winSize *= 2; scale /= 2;
	computeMeasures(waveletImgFloat, waveletImg_mean, waveletImg_variance, waveletImg_std, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_mean, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_variance, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_std, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);

	//HL2
	col_from = 200; col_to = 400; row_from = 0; row_to = 400;
	computeMeasures(waveletImgFloat, waveletImg_mean, waveletImg_variance, waveletImg_std, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_mean, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	//addWaveletFeature(waveletImg_variance, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_std, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);

	//HH2
	col_from = 200; col_to = 400; row_from = 400; row_to = 800;
	computeMeasures(waveletImgFloat, waveletImg_mean, waveletImg_variance, waveletImg_std, row_from, row_to, col_from, col_to, winSize);
	addWaveletFeature(waveletImg_mean, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_variance, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	addWaveletFeature(waveletImg_std, row_from, row_to, col_from, col_to, scale, inputFeatureMatrixVector);
	*/
}

void addWaveletFeature(Mat &waveletFeature , int row_from, int row_to, int col_from, int col_to, int scale, vector<Mat>& inputFeatureMatrixVector) {
	//Retrieve the img

	Mat subImg = waveletFeature(cv::Rect(col_from, row_from, col_to - col_from, row_to - row_from));
	//Upsample
	if (scale % 2 != 0 || scale < 1) {
		cout << "ERROR! addWAveletFeature(): scale is not consistent" << endl;
		return;
	}
	Mat upsampledMatrix;
	pyrUp(subImg, upsampledMatrix, Size(subImg.cols * 2, subImg.rows * 2));
	scale /= 2;
	Mat temp;
	while (scale > 1) {
		scale /= 2;
		pyrUp(upsampledMatrix, temp, Size(upsampledMatrix.cols * 2, upsampledMatrix.rows * 2));
		upsampledMatrix = temp;
	}
	
	//add the upsampled matrix
	inputFeatureMatrixVector.push_back(upsampledMatrix);
}



void computeMeasures1(Mat &waveletImg, Mat &waveletImg_mean, Mat &waveletImg_variance, Mat &waveletImg_std, int row_from, int row_to, int col_from, int col_to, int winSize) {
	float windowSize = winSize * winSize;
	Mat kernel(winSize, winSize, CV_32F, 1);
	kernel = kernel / windowSize;	

	/***SubImg***/
	Mat subImg = waveletImg(cv::Rect(col_from, row_from, col_to - col_from, row_to - row_from));
	/*****Mean*****/
	Mat meanSubImg;
	filter2D(subImg, meanSubImg, subImg.depth(), kernel, Point(-1, -1), BORDER_REFLECT);
	meanSubImg.copyTo(waveletImg_mean.rowRange(row_from, row_to).colRange(col_from, col_to));
	/*****Varience*****/
	//f(r,s) - mean
	//Mat varienceSubImg = Mat(subImg.rows, subImg.cols, CV_32F);
	Mat varienceSubImg = subImg - meanSubImg;
	Mat tmpVarSubImg;
	// (f(r,s)-mean)^2
	pow(varienceSubImg, 2, tmpVarSubImg);
	//finally, variance is:
	filter2D(tmpVarSubImg, varienceSubImg, tmpVarSubImg.depth(), kernel);
	varienceSubImg.copyTo(waveletImg_variance.rowRange(row_from, row_to).colRange(col_from, col_to));
	/***Standard Diviation***/
	Mat stdSubImg;
	sqrt(varienceSubImg, stdSubImg);
	stdSubImg.copyTo(waveletImg_std.rowRange(row_from, row_to).colRange(col_from, col_to));
}

void computeMeasures2(Mat &waveletImg, Mat &energyMat, int row_from, int row_to, int col_from, int col_to, int winSize) {
	float windowSizeSquared = winSize * winSize;
	Mat kernel_Img(winSize, winSize, CV_32F, 1);
	kernel_Img = kernel_Img / windowSizeSquared;

	/***SubImg***/
	Mat subImg = waveletImg(cv::Rect(col_from, row_from, col_to - col_from, row_to - row_from));
	/*****EnergyMeasure*****/
	Mat absSubImg = abs(subImg);
	Mat energySubImg;
	filter2D(absSubImg, energySubImg, subImg.depth(), kernel_Img, Point(-1, -1), BORDER_REFLECT);
	//Smooth over the energy measures
	int kernelSizeForEnergySmoothing = 9;
	Mat energySmoothSubImg;
	filter2D(energySubImg, energySmoothSubImg, subImg.depth(), kernel_Img, Point(-1, -1), BORDER_REFLECT);

	energySmoothSubImg.copyTo(energyMat.rowRange(row_from, row_to).colRange(col_from, col_to));
}