#include "ImageAnalyser.h"
#include <fstream>
#include <iostream>
#include "DataVisualiser.h"

using namespace std;

void ImageAnalyser::convertToGrid(Mat &image, Mat &annotatedImage, vector<int> &organsCount, vector<Mat>& inputFeatureMatrixVector) {
	//calculate new dimensions	
	int newImgWidth = image.cols / gridSize;
	int newImgHeight = image.rows / gridSize;

	Size blurSize(euclideanSize, euclideanSize);

	//create new img / annotatedImg
	Mat dst, dstAnnotated, newImg, newAnnotatedImg;

	GaussianBlur(image, dst, blurSize, 0, 0);
	newImg.create(newImgHeight, newImgWidth, image.type());
	newAnnotatedImg.create(newImgHeight, newImgWidth, image.type());
	//create a duplicate of the inputFeatureMatrixVector with lowerDims
	//also, blur the inputFeatureMatrixVector
	int inputFeatureMatricesCount = inputFeatureMatrixVector.size();
	vector<Mat> gridsizedInputFeatureMatrixVector;
	for (int i = 0; i < inputFeatureMatricesCount; ++i) {
		//1st, blur the current input feature matrix
		Mat& ifm = inputFeatureMatrixVector[i];
		Mat ifm_dst;
		GaussianBlur(ifm, ifm_dst, blurSize, 0, 0);
		inputFeatureMatrixVector[i] = ifm_dst;
		//2nd, create a gridsized matrix and add it to the gridsizedINputFeatureMatrixVector variable
		Mat ifm_gridsized;
		ifm_gridsized.create(newImgHeight, newImgWidth, ifm.type());
		gridsizedInputFeatureMatrixVector.push_back(ifm_gridsized);
	}

	//interate and downsample the information
	for (int imgJ = gridSize / 2, newImgJ = 0; newImgJ < newImgHeight; imgJ += gridSize, ++newImgJ) {
		for (int imgI = gridSize / 2, newImgI = 0; newImgI < newImgWidth; imgI += gridSize, ++newImgI) {
			newImg.at<Vec3b>(newImgJ, newImgI) = dst.at<Vec3b>(imgJ, imgI);
			newAnnotatedImg.at<Vec3b>(newImgJ, newImgI) = annotatedImage.at<Vec3b>(imgJ, imgI);
			//apply to input feature matrices as well
			for (int i = 0; i < inputFeatureMatricesCount; ++i) {
				gridsizedInputFeatureMatrixVector[i].at<float>(newImgJ, newImgI) =
					inputFeatureMatrixVector[i].at<float>(imgJ, imgI);
			}
		}
	}
	image = newImg;
	annotatedImage = newAnnotatedImg;

	inputFeatureMatrixVector.clear();
	//a.insert(a.end(), b.begin(), b.end());
	inputFeatureMatrixVector.insert(inputFeatureMatrixVector.end(), gridsizedInputFeatureMatrixVector.begin(), gridsizedInputFeatureMatrixVector.end());
	
	//reduce the number of organs accordingly
	const int gridSizeSquared = gridSize * gridSize;
	for (int i = 0; i < organsCount.size(); ++i) { organsCount[i] /= gridSizeSquared; }
}

void ImageAnalyser::downsampleUsingGPyramid(Mat &image, Mat &annotatedImage, vector<int> &organsCount) {
	Mat newImg, newAnnotatedImg, tmp;
	pyrDown(image, tmp, Size(image.cols / 2, image.rows / 2));
	pyrDown(tmp, newImg, Size(tmp.cols / 2, tmp.rows / 2));

	pyrDown(annotatedImage, tmp, Size(image.cols / 2, image.rows / 2));
	pyrDown(tmp, newAnnotatedImg, Size(tmp.cols / 2, tmp.rows / 2));

	
	int newImgWidth = image.cols / 4;
	int newImgHeight = image.rows / 4;

	
	for (int imgJ = 4 / 2, newImgJ = 0; newImgJ < newImgHeight; imgJ += 4, ++newImgJ) {
		for (int imgI = 4 / 2, newImgI = 0; newImgI < newImgWidth; imgI += 4, ++newImgI) {
			newAnnotatedImg.at<Vec3b>(newImgJ, newImgI) = annotatedImage.at<Vec3b>(imgJ, imgI);
		}
	}

	image = newImg;
	annotatedImage = newAnnotatedImg;

	//reduce the number of organs accordingly
	const int gridSizeSquared = 4 * 4;
	for (int i = 0; i < organsCount.size(); ++i) { organsCount[i] /= gridSizeSquared; }
}

void ImageAnalyser::addHsv(Mat &BGRimg, vector<Mat>& inputFeatureMatrixVector) {
	Mat image_hsv;
	Mat image_lab;
	cvtColor(BGRimg, image_lab, CV_BGR2HLS);
	std::vector<cv::Mat> hsv_channels;
	//cv::split(image_hsv, hsv_channels);
	cv::split(image_lab, hsv_channels);
	Mat h_image_norm, s_image_norm, v_image_norm;
	hsv_channels[0].convertTo(h_image_norm, CV_32F);
	hsv_channels[1].convertTo(s_image_norm, CV_32F);
	hsv_channels[2].convertTo(v_image_norm, CV_32F);

	inputFeatureMatrixVector.push_back(h_image_norm);
	inputFeatureMatrixVector.push_back(s_image_norm);
	inputFeatureMatrixVector.push_back(v_image_norm);
}

void ImageAnalyser::addHsvBlurred(Mat &BGRimg, vector<Mat>& inputFeatureMatrixVector) {
	Mat image_hsv, image_blurred, image_blurred1;

	//blur(BGRimg, image_blurred, Size(AddHsvBlurred_medianBlurKernalSize, AddHsvBlurred_medianBlurKernalSize), Point(-1, -1), BORDER_REFLECT);
	//blur(image_blurred, image_blurred1, Size(AddHsvBlurred_medianBlurKernalSize, AddHsvBlurred_medianBlurKernalSize), Point(-1, -1), BORDER_REFLECT);
	//blur(image_blurred1, image_blurred, Size(AddHsvBlurred_medianBlurKernalSize, AddHsvBlurred_medianBlurKernalSize), Point(-1, -1), BORDER_REFLECT);

	medianBlur(BGRimg, image_blurred, AddHsvBlurred_medianBlurKernalSize);
	//medianBlur(image_blurred, image_blurred1, AddHsvBlurred_medianBlurKernalSize);
	//medianBlur(image_blurred1, image_blurred, AddHsvBlurred_medianBlurKernalSize);


	cvtColor(image_blurred, image_hsv, CV_BGR2HSV);
	//cvtColor(image_blurred, image_hsv, CV_BGR2XYZ);
	std::vector<cv::Mat> hsv_channels;
	//cv::split(image_hsv, hsv_channels);
	cv::split(image_blurred, hsv_channels);
	Mat h_image_float, s_image_float, v_image_float;
	hsv_channels[0].convertTo(h_image_float, CV_32F);
	hsv_channels[1].convertTo(s_image_float, CV_32F);
	hsv_channels[2].convertTo(v_image_float, CV_32F);

	inputFeatureMatrixVector.push_back(h_image_float);
	inputFeatureMatrixVector.push_back(s_image_float);
	inputFeatureMatrixVector.push_back(v_image_float);
}

void ImageAnalyser::addLab(Mat &BGRimg, vector<Mat>& inputFeatureMatrixVector) {
	Mat image_lab, image_lab_convolved;
	cvtColor(BGRimg, image_lab, CV_BGR2Lab);
	
	std::vector<cv::Mat> lab_channels;
	cv::split(image_lab, lab_channels);
	Mat l_image_norm, a_image_norm, b_image_norm;
	lab_channels[0].convertTo(l_image_norm, CV_32F);
	lab_channels[1].convertTo(a_image_norm, CV_32F);
	lab_channels[2].convertTo(b_image_norm, CV_32F);

	inputFeatureMatrixVector.push_back(l_image_norm);
	inputFeatureMatrixVector.push_back(a_image_norm);
	inputFeatureMatrixVector.push_back(b_image_norm);
}


void ImageAnalyser::addLawsEnergyMeasures(Mat &img, vector<Mat>& inputFeatureMatrixVector) {
	/**Transform into [0,1] float**/
	Mat m_gre;
	cvtColor(img, m_gre, CV_BGR2GRAY);
	m_gre.convertTo(m_gre, CV_32FC1);
	m_gre *= 1. / 255;
	
	extractLaws(m_gre, inputFeatureMatrixVector);
}

void ImageAnalyser::addLawsEnergyMeasuresOnHSV(Mat &img, vector<Mat>& inputFeatureMatrixVector) {
	/***Get HSV into [0, 1]***/
	Mat image_hsv;
	cvtColor(img, image_hsv, CV_BGR2HSV);
	std::vector<cv::Mat> hsv_channels;
	cv::split(image_hsv, hsv_channels);
	Mat h_image_float, s_image_float, v_image_float;
	hsv_channels[0].convertTo(h_image_float, CV_32F);
	hsv_channels[1].convertTo(s_image_float, CV_32F);
	hsv_channels[2].convertTo(v_image_float, CV_32F);
	/**Transform into [0,1] float**/
	h_image_float *= 1. / 179;
	s_image_float *= 1. / 255;
	v_image_float *= 1. / 255;


	extractLaws(h_image_float, inputFeatureMatrixVector);
	extractLaws(s_image_float, inputFeatureMatrixVector);
	extractLaws(v_image_float, inputFeatureMatrixVector);
}

void ImageAnalyser::addLawsEnergyMeasuresGaussianPyramid(Mat &img, vector<Mat>& inputFeatureMatrixVector) {
	int gpMultiplier = 2;
	Mat image_tmp, image_gaussianPyramid;
	/*Gaussian Pyramid*/
	pyrDown(img, image_tmp, Size(img.cols / gpMultiplier, img.rows / gpMultiplier));
	
	/**Transform into [0,1] float**/
	Mat m_gre;
	cvtColor(image_tmp, m_gre, CV_BGR2GRAY);
	m_gre.convertTo(m_gre, CV_32FC1);
	m_gre *= 1. / 255;

	extractLaws(m_gre, inputFeatureMatrixVector);
	//retrieve the energy maps
	Mat em1 = inputFeatureMatrixVector.back(); inputFeatureMatrixVector.pop_back();
	Mat em2 = inputFeatureMatrixVector.back(); inputFeatureMatrixVector.pop_back();
	Mat em3 = inputFeatureMatrixVector.back(); inputFeatureMatrixVector.pop_back();
	Mat em4 = inputFeatureMatrixVector.back(); inputFeatureMatrixVector.pop_back();
	Mat em5 = inputFeatureMatrixVector.back(); inputFeatureMatrixVector.pop_back();
	Mat em6 = inputFeatureMatrixVector.back(); inputFeatureMatrixVector.pop_back();
	Mat em7 = inputFeatureMatrixVector.back(); inputFeatureMatrixVector.pop_back();
	Mat em8 = inputFeatureMatrixVector.back(); inputFeatureMatrixVector.pop_back();
	Mat em9 = inputFeatureMatrixVector.back(); inputFeatureMatrixVector.pop_back();
	
	Mat em1_pu, em2_pu, em3_pu, em4_pu, em5_pu, em6_pu, em7_pu, em8_pu, em9_pu;
	//scale them up
	pyrUp(em1, em1_pu, Size(image_tmp.cols * gpMultiplier, image_tmp.rows * gpMultiplier));
	pyrUp(em2, em2_pu, Size(image_tmp.cols * gpMultiplier, image_tmp.rows * gpMultiplier));
	pyrUp(em3, em3_pu, Size(image_tmp.cols * gpMultiplier, image_tmp.rows * gpMultiplier));
	pyrUp(em4, em4_pu, Size(image_tmp.cols * gpMultiplier, image_tmp.rows * gpMultiplier));
	pyrUp(em5, em5_pu, Size(image_tmp.cols * gpMultiplier, image_tmp.rows * gpMultiplier));
	pyrUp(em6, em6_pu, Size(image_tmp.cols * gpMultiplier, image_tmp.rows * gpMultiplier));
	pyrUp(em7, em7_pu, Size(image_tmp.cols * gpMultiplier, image_tmp.rows * gpMultiplier));
	pyrUp(em8, em8_pu, Size(image_tmp.cols * gpMultiplier, image_tmp.rows * gpMultiplier));
	pyrUp(em9, em9_pu, Size(image_tmp.cols * gpMultiplier, image_tmp.rows * gpMultiplier));

	//add them to the inputFeatureMatrixVector
	inputFeatureMatrixVector.push_back(em1_pu);
	inputFeatureMatrixVector.push_back(em2_pu);
	inputFeatureMatrixVector.push_back(em3_pu);
	inputFeatureMatrixVector.push_back(em4_pu);
	inputFeatureMatrixVector.push_back(em5_pu);
	inputFeatureMatrixVector.push_back(em6_pu);
	inputFeatureMatrixVector.push_back(em7_pu);
	inputFeatureMatrixVector.push_back(em8_pu);
	inputFeatureMatrixVector.push_back(em9_pu);
}

void ImageAnalyser::extractLaws(Mat& oneChannelFloatImg, vector<Mat>& ms_tex_ene_fin) {

	// subtract local average intensity from each pixel
	Mat m_avg = oneChannelFloatImg.clone();
	Size ksize(15, 15);
	/***QUESTION: is blur giving the avg?***/
	blur(oneChannelFloatImg, m_avg, ksize, Point(-1, -1), BORDER_REFLECT);
	Mat m_nor = oneChannelFloatImg - m_avg;
	//	printMatInfo(m_avg);
	//	imshow("2", m_avg);
	//	waitKey(0);
	//	printMatInfo(m_nor);
	//	imshowScaled("2", m_nor);
	//	waitKey(0);

	// create filters
	//todo: this could be done outside, only once; clock it, to see if it's worth
	Mat L5 = (Mat_<float>(1, 5) << 1, 4, 6, 4, 1);
	Mat E5 = (Mat_<float>(1, 5) << -1, -2, 0, 2, 1);
	Mat S5 = (Mat_<float>(1, 5) << -1, 0, 2, 0, -1);
	Mat R5 = (Mat_<float>(1, 5) << 1, -4, 6, -4, 1);
	Mat L5T; transpose(L5, L5T);
	Mat E5T; transpose(E5, E5T);
	Mat S5T; transpose(S5, S5T);
	Mat R5T; transpose(R5, R5T);
	vector<Mat> filters;
	filters.push_back(L5T * E5); //  0
	filters.push_back(E5T * L5); //  1
	filters.push_back(L5T * R5); //  2
	filters.push_back(R5T * L5); //  3
	filters.push_back(E5T * S5); //  4
	filters.push_back(S5T * E5); //  5
	filters.push_back(S5T * S5); //  6
	filters.push_back(R5T * R5); //  7
	filters.push_back(L5T * S5); //  8
	filters.push_back(S5T * L5); //  9
	filters.push_back(E5T * E5); // 10
	filters.push_back(E5T * R5); // 11
	filters.push_back(R5T * E5); // 12
	filters.push_back(S5T * R5); // 13
	filters.push_back(R5T * S5); // 14

	// for each filter
	vector<Mat> ms_tex_ene;
	for (int fi = 0; fi <= 14; ++fi){
		// get texture map
		Mat m_tex;
		Mat filter = filters[fi];
		filter2D(m_nor, m_tex, CV_32F, filter, Point(-1, -1), 0, BORDER_REFLECT);
		//		printMatInfo(m_tex);
		//		imshowScaled("1", m_tex);

		// get texture energy map
		Mat m_tex_abs = abs(m_tex);
		Mat m_tex_ene;
		blur(m_tex_abs, m_tex_ene, ksize, Point(-1, -1), BORDER_REFLECT);
		m_tex_ene = m_tex_ene * (float)ksize.height * (float)ksize.width;
		ms_tex_ene.push_back(m_tex_ene);	
		//		printMatInfo(m_tex_abs);
		//		imshowScaled("1", m_tex_abs);
		//		printMatInfo(m_tex_ene);
		//		imshowScaled("2", m_tex_ene);
		//		waitKey(0);
	}

	// get the final nine texture energy maps

	ms_tex_ene_fin.push_back((ms_tex_ene[0] + ms_tex_ene[1]) / 2.);
	ms_tex_ene_fin.push_back((ms_tex_ene[2] + ms_tex_ene[3]) / 2.);
	ms_tex_ene_fin.push_back((ms_tex_ene[4] + ms_tex_ene[5]) / 2.);
	ms_tex_ene_fin.push_back(ms_tex_ene[6]);
	ms_tex_ene_fin.push_back(ms_tex_ene[7]);
	ms_tex_ene_fin.push_back((ms_tex_ene[8] + ms_tex_ene[9]) / 2.);
	ms_tex_ene_fin.push_back(ms_tex_ene[10]);
	ms_tex_ene_fin.push_back((ms_tex_ene[11] + ms_tex_ene[12]) / 2.);
	ms_tex_ene_fin.push_back((ms_tex_ene[13] + ms_tex_ene[14]) / 2.);
	//	for (int mi = 0; mi < ms_tex_ene_fin.size(); ++mi){
	//		imshowScaled("2", ms_tex_ene_fin[mi]);
	//		waitKey(0);
	//	}
}

void ImageAnalyser::addY(Mat &img, vector<Mat>& inputFeatureMatrixVector) {
	int rows = img.rows;
	int cols = img.cols;

	int topRow = -1;
	int botRow = -1;

	//find top
	for (int j = 0; j < rows; ++j) {
		for (int i = 0; i < cols; ++i) {
			if (img.at<Vec3b>(j, i) == GlobalVariables::COLOR_OFFAL_BACKGROUND)
				continue;
			else {
				topRow = j; break;
			}
		}
	}
	//find bottom
	for (int j = rows-1; j > 0; --j) {
		for (int i = 0; i < cols; ++i) {
			if (img.at<Vec3b>(j, i) == GlobalVariables::COLOR_OFFAL_BACKGROUND)
				continue;
			else {
				botRow = j; break;
			}
		}
	}
	
	int offalPixelsHeight = topRow - botRow;

	Mat yInfo;
	yInfo.create(Size(cols, rows), CV_32F);
	//find top
	for (int j = 0; j < rows; ++j) {
		float currentPerc = j / offalPixelsHeight;
		for (int i = 0; i < cols; ++i) {
			if (img.at<Vec3b>(j, i) == GlobalVariables::COLOR_OFFAL_BACKGROUND)
				yInfo.at<float>(j, i) = -1;
			else {
				yInfo.at<float>(j, i) = currentPerc;
			}
		}
	}
	inputFeatureMatrixVector.push_back(yInfo);
}

void ImageAnalyser::equaliseColorHisto(Mat &bgr_img) {
	cv::Mat lab_image;
	cv::cvtColor(bgr_img, lab_image, CV_BGR2Lab);

	// Extract the L channel
	std::vector<cv::Mat> lab_planes(3);
	cv::split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]

	// apply the CLAHE algorithm to the L channel
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	clahe->setClipLimit(4);
	cv::Mat dst;
	clahe->apply(lab_planes[0], dst);

	// Merge the the color planes back into an Lab image
	dst.copyTo(lab_planes[0]);
	cv::merge(lab_planes, lab_image);

	// convert back to RGB
	cv::Mat image_clahe;
	cv::cvtColor(lab_image, image_clahe, CV_Lab2BGR);

	bgr_img = image_clahe;
}


int ImageAnalyser::gridSize = 5;
int ImageAnalyser::euclideanSize = 15;
int ImageAnalyser::AddHsvBlurred_medianBlurKernalSize = 31;
int ImageAnalyser::globalKernelSize = 25;

string ImageAnalyser::lastProcessedImgFilename = "";
string ImageAnalyser::lastProcessedImgAnnotatedFilename = "";