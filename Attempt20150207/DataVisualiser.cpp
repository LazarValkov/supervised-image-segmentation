#include "DataVisualiser.h"
bool DataVisualiser::isOn = false;
void DataVisualiser::visualiseResults(vector<int>& classifiedResults) {
	int classifiedImages = visualisingData.size();
	for (int i = 0; i < classifiedImages; ++i) {
		visualizeImageResults(visualisingData[i], classifiedResults);
	}
}
void DataVisualiser::visualizeImageResults(VisualisingData& visualisationData, vector<int>& classifiedResults) {

	Mat& annotatedImage = (visualisationData.annotatedImage);
	GlobalVariables::Organ positiveOrgan = visualisationData.positiveOrgan;
	GlobalVariables::Organ negativeOrgans = visualisationData.negativeOrgans;
	int currentIndx = visualisationData.startInd;

	int rowsCount = annotatedImage.rows;
	int colsCount = annotatedImage.cols;

	Vec3b color_Positive = Vec3b(7, 135, 255);
	Vec3b color_Negative = Vec3b(236, 43, 197);

	for (int j = rowsCount - 1; j > -1; j--)
	{
		for (int i = colsCount - 1; i > -1; i--)
		{
			Vec3b annotatedImgIntensity = annotatedImage.at<Vec3b>(j, i);
			if (annotatedImgIntensity == GlobalVariables::COLOR_OFFAL_BACKGROUND) continue;

			GlobalVariables::Organ currentOrgan;

			if (annotatedImgIntensity == GlobalVariables::COLOR_OFFAL_BACKGROUND)
				continue;

			currentOrgan = GlobalVariables::getOrganByColor(annotatedImgIntensity);
						
			if (!(currentOrgan & negativeOrgans || currentOrgan & positiveOrgan))
				continue;
			if (currentIndx >= classifiedResults.size())
				break;
			//annotatedImage.at<Vec3b>(j, i) = currentOrganColor;

			
			if (classifiedResults[currentIndx])
				annotatedImage.at<Vec3b>(j, i) = color_Positive;
			else
				annotatedImage.at<Vec3b>(j, i) = color_Negative;
			
			++currentIndx;
		}
	}

	//imshow("annotatedImage", annotatedImage);
	//imwrite("E:\\edgeSummationVisualisation.png", annotatedImage);
	//waitKey(0);
	//destroyAllWindows();
}

vector<DataVisualiser::VisualisingData> DataVisualiser::visualisingData;