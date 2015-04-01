#ifndef IMAGE_INFO_H
#define IMAGE_INFO_H

using namespace std;
#include <string>

class ImageInfo {
public:

	ImageInfo(string imgFilename, string aImgFilename
		, int aCOthers, int aCHeart, int aCLungs, int aCDiaphragm, int aCLiver) :
		filename_img(imgFilename), filename_aImg(aImgFilename)
		, countOthers(aCOthers), countHeart(aCHeart), countLungs(aCLungs)
		, countDiaphragm(aCDiaphragm), countLiver(aCLiver) {}



	string filename_img, filename_aImg;
	int countOthers, countHeart, countLungs, countDiaphragm, countLiver;
	
};


#endif