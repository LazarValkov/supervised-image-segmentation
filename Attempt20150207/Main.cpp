//#define VISUALISE_DATA
#include "ImageAnalyser.h"
#include <iostream>
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
	string logFilepath = "C:\\temp\\firstLog.txt";
	string fileList_train_debug =	"C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\Train\\filelist_train_d.txt";
	string fileList_train_full =	"C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\Train\\filelist_train_full.txt";
	string fileList_train_full_d_annotations = "C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\Train\\filelist_train_full_d_annotations.txt";
	string fileList_test =			"C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\Train\\filelist_validate_d.txt";

	/*************PIXEL BY PIXEL********************/
	//Test_HSV_PixelByPixel a = Test_HSV_PixelByPixel(true, false);
	//a.run(logFilepath, fileList_train, fileList_test);
	
	//Test_pixel_HSVBlurred tphb = Test_pixel_HSVBlurred(true, false, GlobalVariables::LIVER);
	//tphb.run(logFilepath, fileList_train, fileList_test);

	//Test_grid_HSV t2 = Test_grid_HSV(true, false);
	//t2.run(logFilepath, fileList_train, fileList_test);
	
	//Test_grid_LAB t3 = Test_grid_LAB(true, false, GlobalVariables::LIVER);
	//t3.run(logFilepath, fileList_train, fileList_test);


	//Test_grid_HSV_texture t3 = Test_grid_HSV_texture(true, false, GlobalVariables::DIAPHRAGME);
	//t3.run(logFilepath, fileList_train, fileList_test);

	//Test_pixel_HSV_texture t4 = Test_pixel_HSV_texture(false, true, GlobalVariables::DIAPHRAGME);
	//t4.run(logFilepath, fileList_train, fileList_test);

	//..Test_pixel_HSVBlurred_Texture tphsvbt = Test_pixel_HSVBlurred_Texture(false, true, GlobalVariables::LIVER);
	//..tphsvbt.run(logFilepath, fileList_train, fileList_test);
	
	//Test_grid_HSVBlurred_HSVTextures tghsvbhsvt = Test_grid_HSVBlurred_HSVTextures(true, false, GlobalVariables::DIAPHRAGME);
	//tghsvbhsvt.run(logFilepath, fileList_train, fileList_test);

	/*Test_grid_HSV_AB t2 = Test_grid_HSV_AB(true, false);
	t2.run(logFilepath, fileList_train, fileList_test);*/
	
	
	CrossValidation crossVal;

	//Test_grid_colorBlurred* tgcb = new Test_grid_colorBlurred(true, true, GlobalVariables::LUNGS);
	//crossVal.kfold(10, logFilepath, fileList_train_full, tgcb);

	Test_grid_HSVblurred_Laws_Wavelets* tghsvblw = new Test_grid_HSVblurred_Laws_Wavelets(false, true, GlobalVariables::LUNGS);
	crossVal.kfold(2, logFilepath, fileList_train_debug, tghsvblw);


	//Test_grid_laws* tglaws = new Test_grid_laws(true, false, GlobalVariables::LUNGS);
	//crossVal.kfold(10, logFilepath, fileList_train_full, tglaws);

	//Test_grid_wavelets* tgwavelets = new Test_grid_wavelets(true, false, GlobalVariables::LUNGS);
	//crossVal.kfold(10, logFilepath, fileList_train_full, tgwavelets);

	//Test_pixel_HSVBlurred_HSVTextures* tphbht = new Test_pixel_HSVBlurred_HSVTextures(false, true, GlobalVariables::DIAPHRAGME);
	//crossVal.kfold(2, logFilepath, fileList_train_full, tphbht);
	
	
	//Test_pixel_HSVBlurred_TP* tphbhtp = new Test_pixel_HSVBlurred_TP(true, false, GlobalVariables::LUNGS);
	//crossVal.kfold(2, logFilepath, fileList_train_full, tphbhtp);
	
	//Test_grid_HSV* tgshv = new Test_grid_HSV(true, false, GlobalVariables::Organ::HEART);
	//crossVal.kfold(10, logFilepath, fileList_train_full, tgshv);

	//Test_grid_HSV_texture* tgshvt = new Test_grid_HSV_texture(true, false, GlobalVariables::Organ::HEART);
	//crossVal.kfold(2, logFilepath, fileList_train_full, tgshvt);

	/*
	ImageAnalyser::IA_Functions iaFuncs = ImageAnalyser::IA_Functions::ADD_HSV_BLURRED | ImageAnalyser::IA_Functions::ADD_LAWS_ENERGY_MEASURES;
	string testDebugListFilename = "C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\Train\\filelist_validate_d.txt";
	string filename_liverClassifier = "C:\\temp\\Test_pixel_HSVBlurred_Texture.txt";
	string filename_diaphragmClassifier = "C:\\temp\\Test_pixel_HSVBlurred_Texture_diaphragm.txt";
	BottomsUp bu = BottomsUp(filename_liverClassifier, filename_diaphragmClassifier);

	string filename_1img = "C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\Train\\DSC_4368.png";
	string filename_1imgA = "C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\Train\\DSC_4368_a.png";


	bu.run(fileList_train, testDebugListFilename, iaFuncs, 12, 1);
	bu.performClassificiation(filename_1img, filename_1imgA);
	*/
	system("pause");
}