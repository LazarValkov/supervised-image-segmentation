#include <iostream>
#include <fstream>
#pragma once
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class StudentsTTests {
public:
	
	//Compares two classification results read from a file
	//using Paired t-test, two-tailed test
	static bool compareClassificationResults(std::string fileName, int lineNumber_CurrentBestPerformance, int lineNumber_CompatitorPerformance) {
		string tempLine, line_currentBestPerformance, line_competitorsPerformance;
		ifstream myfile(fileName);
		if (!myfile.is_open())
			throw exception("file couldn't be opened!");
		
		int currentLineNum = 0;
		while (getline(myfile, tempLine))
		{
			++currentLineNum;
			if (currentLineNum == lineNumber_CurrentBestPerformance)
				line_currentBestPerformance = tempLine;
			if (currentLineNum == lineNumber_CompatitorPerformance)
				line_competitorsPerformance = tempLine;
			//stop if we have read both lines
			if (line_currentBestPerformance.size() > 0 && line_competitorsPerformance.size() > 0)
				break;
		}
		myfile.close();
		
		vector<float> numbers_CurrentBest = getNumbersInALine(line_currentBestPerformance);
		vector<float> numbers_Competitor = getNumbersInALine(line_competitorsPerformance);

		if (numbers_CurrentBest[0] != numbers_Competitor[0])
			throw exception("the test cases from both classifiers do not match.");

		int numOfTests = numbers_Competitor[0];
		int lastTestResultIndex = numOfTests * 2;

		float sum_difference_accuracy = 0, sum_difference_fMeasure = 0;
		for (int i = 1; i <= lastTestResultIndex; ++i) {
			float difference = numbers_CurrentBest[i] - numbers_Competitor[i];
			if (i % 2 == 1) {
				sum_difference_accuracy += difference;
			}
			else {
				sum_difference_fMeasure += difference;
			}
		}
		float mean_difference_accuracy = sum_difference_accuracy / numOfTests;
		float mean_difference_fmeasure = sum_difference_fMeasure / numOfTests;

		sum_difference_accuracy = sum_difference_fMeasure = 0;
		for (int i = 1; i <= lastTestResultIndex; ++i) {
			float difference = numbers_CurrentBest[i] - numbers_Competitor[i];
			if (i % 2 == 1) {
				sum_difference_accuracy += pow(difference-mean_difference_accuracy, 2);
			}
			else {
				sum_difference_fMeasure += pow(difference-mean_difference_fmeasure, 2);
			}
		}

		float std_deviation_accuracy = sqrt(sum_difference_accuracy / numOfTests);
		float std_deviation_fmeasure = sqrt(sum_difference_fMeasure / numOfTests);


		float t_accuracy = (mean_difference_accuracy / std_deviation_accuracy) * sqrt(numOfTests);
		float t_fMeasure = (mean_difference_fmeasure / std_deviation_fmeasure) * sqrt(numOfTests);

		//Currecntly the criticalTValue is hard coded.
		float criticalTValue = 4.604f;
		if (criticalTValue < abs(t_accuracy) ) {
			cout << "The difference in accuracy is statistically signifficant!" << endl;
		}
		else {
			cout << "The difference in accuracy is NOT statistically signifficant!" << endl;
		}
		if (criticalTValue < abs(t_fMeasure)) {
			cout << "The difference in fMeasure is statistically signifficant!" << endl;
		}
		else {
			cout << "The difference in fMeasure is NOT statistically signifficant!" << endl;
		}

		return false;
	}
private:
	static vector<float> getNumbersInALine(string line) {
		istringstream iss(line);
		vector<float> numbers;
		do
		{
			string sub;
			iss >> sub;
			if (sub.size() == 0)
				continue;
			float num = std::stof(sub);
			numbers.push_back(num);
		} while (iss);
		return numbers;
	}
};