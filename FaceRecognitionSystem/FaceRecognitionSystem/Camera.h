#pragma once
#include "libcurl/include/curl/curl.h"
#include "string"
using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "libcurl/lib/libcurl_a_debug.lib")
#else
#pragma comment(lib, "libcurl/lib/libcurl_a.lib")
#endif

class Camera
{
	CURL *curl;
	string IPAddress;
	string USERPWD;

public:
	Camera(string IPAddress, string USERPWD);
	~Camera();
	void sendMessage(int command);
};

