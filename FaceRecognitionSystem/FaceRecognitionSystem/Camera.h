#pragma once

#include "string"
#include <filesystem>
#include "opencv2\core\core.hpp"
#include <ctime>
#include <iostream>
#include <thread>
#include "Utilities.h"
#include "ImgProc.h"

#define CURL_STATICLIB
#include "curl/curl.h"

#ifdef _DEBUG
#pragma comment(lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment(lib, "curl/libcurl_a.lib")
#endif

#define VK_U			'w'
#define VK_D			's'
#define VK_L			'a'
#define VK_R			'd'
#define VK_HOME			'h'
#define VK_PICTURE		'p'
#define VK_COMMAND		'c'
#define VK_ESTIMATE		'e'
#define VK_TRAIN_FR		't'
#define VK_FR			'r'
#define VK_EXIT			'q'

class Camera
{
	CURL *curl;
	std::string IPAddress;
	std::string USERPWD;
	ImgProc *imgproc;
	double getFPS();
public:
	Camera(std::string IPAddress, std::string USERPWD);
	~Camera();

	std::string getIPaddress() { return IPAddress; }
	std::string getUSERPWD() { return USERPWD; }
	void setIPaddress(std::string IPAddr) { IPAddress = IPAddr; }
	void setUSERPWD(std::string UserPwd) { USERPWD = UserPwd; }
	void CaptureFrame();
	void CaptureFrameToCorp();
	void sendMessage(char command);
	Camera* typeCameraData();
	bool testConnectionCurl();
	void testConnection(Camera *& camera);
};

