#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;
using namespace cv;

void processImage(ofstream &ofs, const string &imgFilename, const string &annotatedImageFilename);
const Vec3b COLOR_OTHERS = Vec3b(0, 255, 255);
const Vec3b COLOR_HEART = Vec3b(255, 0, 0);
const Vec3b COLOR_LUNGS = Vec3b(0, 255, 0);
const Vec3b COLOR_DIAPHRAGM = Vec3b(255, 255, 0);
const Vec3b COLOR_LIVER = Vec3b(0, 0, 255);

int main(int argc, char* argv[])
{
	string fileList = "C:\\D\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\All\\filelist.txt";
	string fileList_new = fileList + "_new.txt";
	ifstream ifs(fileList);
	ofstream ofs;
	ofs.open(fileList_new);


	string totalImagesCountStr, imgWidth, imgHeight;
	getline(ifs, totalImagesCountStr); getline(ifs, imgWidth); getline(ifs, imgHeight);
	int numOfListedImages = atoi(totalImagesCountStr.c_str());
	int widthOfListedImages = atoi(imgWidth.c_str());
	int heightOfListedImages = atoi(imgHeight.c_str());

	ofs << numOfListedImages << "\n" << widthOfListedImages << "\n" << heightOfListedImages << "\n";

	/*Now populate them*/
	string imageFilename;
	string annotatedImageFilename;

	while (getline(ifs, imageFilename) && getline(ifs, annotatedImageFilename))
	{
		cout << "processing " << imageFilename << endl;
		processImage(ofs, imageFilename, annotatedImageFilename);
	}

	ifs.close();
	ofs.close();
	system("pause");
	return 0;
}

void processImage(ofstream &ofs, const string &imgFilename, const string &annotatedImageFilename) {
	ofs << imgFilename << "\n";
	ofs << annotatedImageFilename << "\n";

	Mat annotatedImage = imread(annotatedImageFilename);
	int rows = annotatedImage.rows;
	int cols = annotatedImage.cols;

	int others = 0, heart = 0, lungs = 0, diaphragm = 0, liver = 0;

	for (int j = 0; j < rows; ++j) {
		for (int i = 0; i < cols; ++i) {
			if (annotatedImage.at<Vec3b>(j, i) == COLOR_OTHERS) {
				++others;
			}
			if (annotatedImage.at<Vec3b>(j, i) == COLOR_HEART) {
				++heart;
			}
			if (annotatedImage.at<Vec3b>(j, i) == COLOR_LUNGS) {
				++lungs;
			}
			if (annotatedImage.at<Vec3b>(j, i) == COLOR_DIAPHRAGM) {
				++diaphragm;
			}
			if (annotatedImage.at<Vec3b>(j, i) == COLOR_LIVER) {
				++liver;
			}
		}
	}
	ofs << others << "\n" << heart << "\n" << lungs << "\n" << diaphragm << "\n" << liver << "\n";
}