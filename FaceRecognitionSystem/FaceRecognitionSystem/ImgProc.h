#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/objdetect/objdetect.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/contrib/contrib.hpp"
#include <string> 
#include <iostream> 
#include <filesystem>
#include <fstream>
#include <conio.h>
#include "Utilities.h"
#include <map>
#include <thread>
#include <atomic>

class ImgProc
{
	std::map <int, std::string> peopleBase; //people from picture base in /screenshots/... <label, folder name>
	std::map <int, std::string> predictedPeople; //people predicted by algorithm
	std::string face_cascade_name;
	cv::CascadeClassifier face_cascade;        
	std::string window_name;
	int detectFace(cv::Mat img);
	bool isModelTrained;
public:
	cv::Ptr<cv::FaceRecognizer> model;
	std::vector<cv::Mat> images;
	std::vector<int> labels;
	ImgProc();
	~ImgProc();
	bool createCSV();
	void countPeople(std::string userPwd, std::string addressIP);
	void read_csv(const std::string & filename, std::vector<cv::Mat> & images, std::vector<int> & labels);
	void ImgProc::TrainFaceRecognizer();
	void predictPerson(std::string userPwd, std::string addressIP);
	cv::Mat cropFace(cv::Mat img);
};

