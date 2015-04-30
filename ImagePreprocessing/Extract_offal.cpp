#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

void extractOffal(Mat& normalImage, Mat& annotatedImage);
void processImage(char* filename, char* afilename);
const string outputDir = "E:\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\OffalOnly\\";
int main32(int argc, char* argv[])
{
	Vec3b a(255, 255, 255);
	Vec3b b(255, 255, 255);
	Vec3b c(255, 255, 0);

	cout << (a == b) << " " << (a == c) << endl;

	/*
	const char* img_filename = "E:\\Work\\University\\Stage 3\\final year project\\C++ Workspace\\imageExamples\\DSC_4295.png";
	const char* aImg_filename = "E:\\Work\\University\\Stage 3\\final year project\\C++ Workspace\\imageExamples\\DSC_4295_a.png";
	const char* outputImgFilename = "E:\\Work\\University\\Stage 3\\final year project\\C++ Workspace\\imageExamples\\DSC_4295_o.png";



	ifstream ifs("E:\\Work\\University\\Stage 3\\final year project\\data\\annotatedImagesOnly\\filelist.txt");
	string line1;
	string line2;

	while (getline(ifs, line1) && getline(ifs, line2))
	{
	char * cline = new char[line1.length() + 1];
	strcpy(cline, line1.c_str());

	char * cline2 = new char[line2.length() + 1];
	strcpy(cline2, line2.c_str());

	processImage(cline, cline2);

	delete cline;
	delete cline2;
	}*/


	/*
	Mat Image, aImage, oImage;

	Image = imread(img_filename, CV_LOAD_IMAGE_COLOR);
	aImage = imread(aImg_filename, CV_LOAD_IMAGE_COLOR);




	extractOffal(Image, aImage, oImage);

	imwrite(outputImgFilename, Image);
	*/
	system("pause");
	return 0;
}

void processImage(char* filename, char* afilename) {
	Mat Image, aImage;

	Image = imread(filename, CV_LOAD_IMAGE_COLOR);
	aImage = imread(afilename, CV_LOAD_IMAGE_COLOR);

	extractOffal(Image, aImage);

	string outputImgFilename = outputDir + filename[71] + filename[72] + filename[73] + filename[74] + filename[75] + filename[76] + filename[77] + filename[78] + ".png";
	cout << outputImgFilename << endl;
	imwrite(outputImgFilename, Image);
}

void extractOffal(Mat& normalImage, Mat& aImage) {
	CV_Assert(normalImage.depth() == CV_8U);  // accept only uchar images

	/*Vec3b aIntensity = aImage.at<Vec3b>(10, 15);
	int blue = aIntensity.val[0];
	int green = aIntensity.val[1];
	int red = aIntensity.val[2];
	cout << "Intensity = " << endl << " " << blue << " " << green << " " << red << endl << endl;*/

	for (int i = 0; i < normalImage.rows; i++)
	{
		for (int j = 0; j < normalImage.cols; j++)
		{
			Vec3b aIntensity = aImage.at<Vec3b>(i, j);
			int blue = aIntensity.val[0];
			int green = aIntensity.val[1];
			int red = aIntensity.val[2];
			if (blue == green && blue == red && red == green && blue == 255)
			{
				normalImage.at<Vec3b>(i, j)[0] = 255;
				normalImage.at<Vec3b>(i, j)[1] = 255;
				normalImage.at<Vec3b>(i, j)[2] = 255;
			}
			/*normalImage.at<Vec3b>(i, j)[0] = 255;
			normalImage.at<Vec3b>(i, j)[1] = 255;
			normalImage.at<Vec3b>(i, j)[2] = 255;*/
		}
	}
	//namedWindow("Modify pixel", CV_WINDOW_AUTOSIZE);
	//imshow("Modify pixel", normalImage);
}
