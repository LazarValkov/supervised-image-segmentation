#include "DataVisualiser.h"
Mat* DataVisualiser::imgAnnotated_gridsized = new Mat();

void DataVisualiser::visualizeClassifiedFromBottomToTop(vector<int>& classifiedResults, string annotatedImageFilePath) {

	/*Load annotated image*/
	//Mat annotatedImage = imread(annotatedImageFilePath);
	Mat& annotatedImage = *imgAnnotated_gridsized;

	int rowsCount = annotatedImage.rows;
	int colsCount = annotatedImage.cols;

	int currentEntry = 0;

	for (int j = rowsCount - 1; j > -1; j--)
	{
		for (int i = colsCount - 1; i > -1; i--)
		{
			Vec3b intensity = annotatedImage.at<Vec3b>(j, i);
			/**Modify only offal pixels**/
			if (intensity == GlobalVariables::COLOR_OFFAL_BACKGROUND) continue;
			//debug: ignore liver and diaphragm, too
			if (intensity == GlobalVariables::COLOR_DIAPHRAGM || intensity == GlobalVariables::COLOR_LIVER)  {
				continue;
			}


			if (currentEntry >= classifiedResults.size())
				break;
			annotatedImage.at<Vec3b>(j, i) = GlobalVariables::ANNOTATED_ORGAN_COLORS[classifiedResults[currentEntry++]];
		}
	}

	//imshow("annotatedImage", annotatedImage);
	//imwrite("E:\\edgeSummationVisualisation.png", annotatedImage);
	//waitKey(0);
	//destroyAllWindows();
}