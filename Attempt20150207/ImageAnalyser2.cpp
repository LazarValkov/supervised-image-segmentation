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

	Mat waveletImgInt = imread(waveletDir + waveletFilename, 0);

	/************Compute the measures****************/
	Mat waveletImgFloat;
	waveletImgInt.convertTo(waveletImgFloat, CV_32F);
	Mat waveletImg_energy;
	waveletImg_energy.create(waveletImgFloat.size(), waveletImgFloat.type());
	
	//LL3
	int col_from = 0, col_to = 100, row_from = 0, row_to = 200, scale = 8, winSize = globalKernelSize / scale;
	winSize = winSize % 2 == 1 ? winSize : winSize - 1;

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

	/***SubImg***/
	Mat subImg = waveletImg(cv::Rect(col_from, row_from, col_to - col_from, row_to - row_from));
	/*****Mean*****/
	Mat meanSubImg;
	blur(subImg, meanSubImg, Size(winSize, winSize), Point(-1, -1), BORDER_REFLECT);
	meanSubImg.copyTo(waveletImg_mean.rowRange(row_from, row_to).colRange(col_from, col_to));
	/*****Varience*****/
	//f(r,s) - mean
	//Mat varienceSubImg = Mat(subImg.rows, subImg.cols, CV_32F);
	Mat varienceSubImg = subImg - meanSubImg;
	Mat tmpVarSubImg;
	// (f(r,s)-mean)^2
	pow(varienceSubImg, 2, tmpVarSubImg);
	//finally, variance is:
	blur(tmpVarSubImg, varienceSubImg, Size(winSize, winSize), Point(-1, -1), BORDER_REFLECT);
	varienceSubImg.copyTo(waveletImg_variance.rowRange(row_from, row_to).colRange(col_from, col_to));
	/***Standard Diviation***/
	Mat stdSubImg;
	sqrt(varienceSubImg, stdSubImg);
	stdSubImg.copyTo(waveletImg_std.rowRange(row_from, row_to).colRange(col_from, col_to));
}

void computeMeasures2(Mat &waveletImg, Mat &energyMat, int row_from, int row_to, int col_from, int col_to, int winSize) {
	/***SubImg***/
	Mat subImg = waveletImg(cv::Rect(col_from, row_from, col_to - col_from, row_to - row_from));
	/*****EnergyMeasure*****/
	Mat absSubImg = abs(subImg);
	Mat energySubImg;
	blur(absSubImg, energySubImg, Size(winSize, winSize), Point(-1, -1), BORDER_REFLECT);
	//Smooth over the energy measures
	int kernelSizeForEnergySmoothing = 9;
	Mat energySmoothSubImg;
	blur(energySubImg, energySmoothSubImg, Size(winSize, winSize), Point(-1, -1), BORDER_REFLECT);
	energySmoothSubImg.copyTo(energyMat.rowRange(row_from, row_to).colRange(col_from, col_to));
}

void homogeinity_addEnergy(Mat& img, vector<Mat>& inputFeatureMatrixVector, int kernelSize, Mat& homogeinityMap);
void getUpsampledMatrix(Mat &waveletFeature, int row_from, int row_to, int col_from, int col_to, int scale, Mat& dst);
float getHomogeneityIndex(Mat& blurredGrayImg, Point& topLeftCorner, int regionSize);
Mat getLowestHomogeneityMap(Mat& grayscaleImg, int kernelSize);


void ImageAnalyser::addWaveletMeasures_homogeinity(Mat& grayscaledImg, string& waveletFilename, vector<Mat>& inputFeatureMatrixVector) {
	Mat waveletImgInt = imread(waveletDir + waveletFilename, 0);
	int hw_kernelSize = 17;
	
	/************Compute the measures****************/
	Mat waveletImgFloat;
	waveletImgInt.convertTo(waveletImgFloat, CV_32F);
	
	Mat lowestHomogeneityMap = getLowestHomogeneityMap(grayscaledImg, hw_kernelSize);

	//LL3
	int col_from = 0, col_to = 100, row_from = 0, row_to = 200, scale = 8;
	Mat dst_LL3;
	getUpsampledMatrix(waveletImgFloat, row_from, row_to, col_from, col_to, scale, dst_LL3);
	homogeinity_addEnergy(dst_LL3, inputFeatureMatrixVector, hw_kernelSize, lowestHomogeneityMap);

	//HL3
	col_from = 100; col_to = 200; row_from = 0; row_to = 200;
	Mat dst_HL3;
	getUpsampledMatrix(waveletImgFloat, row_from, row_to, col_from, col_to, scale, dst_HL3);
	homogeinity_addEnergy(dst_HL3, inputFeatureMatrixVector, hw_kernelSize, lowestHomogeneityMap);

	//LH3
	col_from = 0; col_to = 100; row_from = 200; row_to = 400;
	Mat dst_LH3;
	getUpsampledMatrix(waveletImgFloat, row_from, row_to, col_from, col_to, scale, dst_LH3);
	homogeinity_addEnergy(dst_LH3, inputFeatureMatrixVector, hw_kernelSize, lowestHomogeneityMap);

	//HH3
	col_from = 100; col_to = 200; row_from = 200; row_to = 400;
	Mat dst_HH3;
	getUpsampledMatrix(waveletImgFloat, row_from, row_to, col_from, col_to, scale, dst_HH3);
	homogeinity_addEnergy(dst_HH3, inputFeatureMatrixVector, hw_kernelSize, lowestHomogeneityMap);
}

void homogeinity_addEnergy(Mat& img, vector<Mat>& inputFeatureMatrixVector, int kernelSize, Mat& homogeinityMap) {
	int regionSize = kernelSize / 2 + 1;
	//debug
	//img *= 1. / 255;
	Mat absSubImg = abs(img);
	Mat blurredImg;
	blur(absSubImg, blurredImg, Size(regionSize, regionSize), Point(-1, -1), BORDER_REFLECT);

	int rows = img.rows;
	int cols = img.cols;

	for (int j = 0; j < rows; ++j) {
		for (int i = 0; i < cols; ++i) {
			Vec2i coordinates = homogeinityMap.at<Vec2i>(j, i);
			img.at<float>(j, i) = blurredImg.at<float>(coordinates[0], coordinates[1]);
		}
	}
	inputFeatureMatrixVector.push_back(img);

}

void getUpsampledMatrix(Mat &waveletFeature, int row_from, int row_to, int col_from, int col_to, int scale, Mat& dst) {
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

	dst = upsampledMatrix;
}

Mat getLowestHomogeneityMap(Mat& grayscaleImg, int kernelSize) {
	Mat map = Mat(grayscaleImg.size(), CV_32SC2);
	//CV_8SC2
	
	int rows = grayscaleImg.rows, cols = grayscaleImg.cols;
	int regionSize = kernelSize / 2 + 1;
	int subRegionSize = regionSize / 2 + 1;
	
	Mat grayscaleImg_float;
	grayscaleImg.convertTo(grayscaleImg_float, CV_32FC1);
	Mat blurredGrayImg;
	blur(grayscaleImg_float, blurredGrayImg, Size(subRegionSize, subRegionSize), Point(-1, -1), BORDER_REFLECT);

	for (int j = 0; j < rows; ++j) {
		for (int i = 0; i < cols; ++i) {
			Point topLeftCorner_topLeftRegion = Point(i-regionSize+1, j-regionSize+1);
			Point topLeftCorner_topRightRegion = Point(i, j - regionSize + 1);
			Point topLeftCorner_bottomLeftRegion = Point(i - regionSize + 1, j);
			Point topLeftCorner_bottomRightRegion = Point(i, j);
			Point topLeftCorenr_centerRegion = Point(i - subRegionSize + 1, j - subRegionSize + 1);

			Point min_Point = topLeftCorner_topLeftRegion;
			float min = getHomogeneityIndex(blurredGrayImg, topLeftCorner_topLeftRegion, regionSize);
			
			float tmp = getHomogeneityIndex(blurredGrayImg, topLeftCorner_topRightRegion, regionSize);
			if (tmp < min) {
				min = tmp;
				min_Point = topLeftCorner_topRightRegion;
			}

			tmp = getHomogeneityIndex(blurredGrayImg, topLeftCorner_bottomLeftRegion, regionSize);
			if (tmp < min) {
				min = tmp;
				min_Point = topLeftCorner_bottomLeftRegion;
			}

			tmp = getHomogeneityIndex(blurredGrayImg, topLeftCorner_bottomRightRegion, regionSize);
			if (tmp < min) {
				min = tmp;
				min_Point = topLeftCorner_bottomRightRegion;
			}

			tmp = getHomogeneityIndex(blurredGrayImg, topLeftCorenr_centerRegion, regionSize);
			if (tmp < min) {
				min = tmp;
				min_Point = topLeftCorenr_centerRegion;
			}

			
			int y = min_Point.y + subRegionSize - 1;
			int x = min_Point.x + subRegionSize - 1;

			
			Vec2i dbg = Vec2i(y, x);
			map.at<Vec2i>(j, i) = dbg;
		}
	}
	return map;
}

//regionSize must be 5, 9, 17 etc...
float getHomogeneityIndex(Mat& blurredGrayImg, Point& topLeftCorner, int regionSize) {
	int rows = blurredGrayImg.rows;
	int cols = blurredGrayImg.cols;

	int subRegionSize = regionSize / 2 + 1;
	int halfOfSubRegionSize = subRegionSize / 2;

	if (topLeftCorner.x < 0 || topLeftCorner.x >= cols || topLeftCorner.y < 0 || topLeftCorner.y >= rows
		|| topLeftCorner.x + regionSize > cols || topLeftCorner.y + regionSize > rows) {
		return INT_MAX;
	}

	Point center = Point(topLeftCorner.x + subRegionSize - 1, topLeftCorner.y + subRegionSize - 1);
	
	float AV1 = blurredGrayImg.at<float>(center.y - halfOfSubRegionSize, center.x - halfOfSubRegionSize);
	float AV2 = blurredGrayImg.at<float>(center.y - halfOfSubRegionSize, center.x + halfOfSubRegionSize);
	float AV3 = blurredGrayImg.at<float>(center.y + halfOfSubRegionSize, center.x - halfOfSubRegionSize);
	float AV4 = blurredGrayImg.at<float>(center.y + halfOfSubRegionSize, center.x + halfOfSubRegionSize);

	return abs(AV1 + AV4 - AV2 - AV3)
		+ abs(AV3+AV4 - AV1 - AV2);
}