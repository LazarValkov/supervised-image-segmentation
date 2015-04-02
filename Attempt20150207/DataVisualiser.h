#ifndef DATA_VISUALISER
#define DATA_VISUALISER
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "GlobalVariables.h"
#include <vector>

using namespace cv;
using namespace std;
class DataVisualiser {
public:
	struct VisualisingData {
		VisualisingData(Mat aImg, GlobalVariables::Organ posOrg, GlobalVariables::Organ negOrg, int beginInd)
			: annotatedImage(aImg), positiveOrgan(posOrg), negativeOrgans(negOrg), startInd(beginInd) {}
		Mat annotatedImage;
		GlobalVariables::Organ positiveOrgan;
		GlobalVariables::Organ negativeOrgans;
		int startInd;
	};
	
	static vector<VisualisingData> visualisingData;
	static bool isOn;
	static void visualiseResults(vector<int>& classifiedResults);
private:
	static void visualizeImageResults(VisualisingData& visualisingData, vector<int>& classifiedResults);
};
#endif