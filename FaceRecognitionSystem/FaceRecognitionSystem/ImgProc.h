#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/objdetect/objdetect.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 
#include <string> 
#include <iostream> 
#include <filesystem>
#include <fstream>

class ImgProc
{
	std::string face_cascade_name;
	cv::CascadeClassifier face_cascade;        
	std::string window_name;
	int detectFace(cv::Mat img);
public:
	ImgProc();
	~ImgProc();
	bool createCSV();
	void countPeople(std::string userPwd, std::string addressIP);
	void read_csv(const std::string & filename, std::vector<cv::Mat> & images, std::vector<int> & labels);

};

