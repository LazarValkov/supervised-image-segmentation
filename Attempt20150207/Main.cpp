#include "ImageAnalyser.h"
#include <iostream>
#include "DataVisualiser.h"
#include "Tester.h"
#include "Test_HSV_PixelByPixel.h"
#include "Test_grid_HSV.h"
#include "Test_grid_HSV_texture.h"
#include "Test_pixel_HSV_texture.h"
#include "Test_grid_HSV_AB.h"
#include "Test_grid_LAB.h"
#include "Test_pixel_HSVBlurred.h"
#include "Test_pixel_HSVBlurred_Texture.h"
#include "Test_grid_HSVBlurred_HSVTextures.h"
#include "Test_pixel_HSVBLurred_HSVTextures.h"
#include "Test_HSVB_TP.h"
#include "GlobalVariables.h"
#include "CrossValidation.h"
#include "Test_grid_colorBlurred.h"
#include "Test_grid_Laws.h"
#include "Test_grid_wavelets.h"
#include "Test_grid_HSVblurred_Laws_Wavelets.h"

#include "BottomsUp.h"

using namespace std;

void main() {
	//
	string logFilepath = "C:\\temp\\firstLog.txt";
	string fileList_train_debug =	"C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\Train\\filelist_train_d.txt";
	string fileList_train_full =	"C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\Train\\filelist_train_full.txt";
	string fileList_train_full_d_annotations = "C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\Train\\filelist_train_full_d_annotations.txt";
	string fileList_test =			"C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\Train\\filelist_validate_d.txt";

	GlobalVariables::Organ positiveOrgan = GlobalVariables::LUNGS;
	GlobalVariables::Organ negativeOrgans = GlobalVariables::HEART | GlobalVariables::OTHERS;

	DataVisualiser::isOn = true;

	CrossValidation crossVal;

	Test_grid_wavelets* tgwlts = new Test_grid_wavelets(true, false, positiveOrgan, negativeOrgans);
	crossVal.kfold(5, logFilepath, fileList_train_full, tgwlts);

	//Test_grid_colorBlurred* tgcb = new Test_grid_colorBlurred(false, true, positiveOrgan, negativeOrgans);
	//crossVal.kfold(2, logFilepath, fileList_train_debug, tgcb);
	
	//Test_grid_HSVblurred_Laws_Wavelets* tghsvblw = new Test_grid_HSVblurred_Laws_Wavelets(false, true, positiveOrgan, negativeOrgans);
	//crossVal.kfold(2, logFilepath, fileList_train_debug, tghsvblw);

	system("pause");
}