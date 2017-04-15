#pragma once
#include "libcurl/include/curl/curl.h"
#include "string"
#include <filesystem>
#include "opencv2\highgui\highgui.hpp"
#include <ctime>
#include <iostream>
#include <thread>
#include "Utilities.h"

#ifdef _DEBUG
#pragma comment(lib, "libcurl/lib/libcurl_a_debug.lib")
#else
#pragma comment(lib, "libcurl/lib/libcurl_a.lib")
#endif

#define VK_HOME 0x48
#define VK_PICTURE 0x50
#define VK_COMMAND 0x43

class Camera
{
	CURL *curl;
	std::string IPAddress;
	std::string USERPWD;

public:
	Camera(std::string IPAddress, std::string USERPWD);
	~Camera();
	bool testConnection();
	void captureFrame();
	void sendMessage(int command);
	Camera* typeCameraData();
};

