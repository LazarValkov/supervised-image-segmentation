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
	/**Only for DEBUG**/
	static Mat* imgAnnotated_gridsized;
	static void DataVisualiser::visualizeClassifiedFromBottomToTop(vector<int>& classifiedResults, string annotatedImageFilePath);

};
#endif