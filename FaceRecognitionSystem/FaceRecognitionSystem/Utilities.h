#pragma once
#include <conio.h>
#include <iostream>
#include <climits>
#include <regex>
#include "Camera.h"
#include <sstream>
#include <vector>

class Utilities
{
public:
	Utilities();
	~Utilities();
	static void printCommandList();
	static void cleanBuffer();
	static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
	static void maskPassword(std::string &password);
	static std::vector<std::string> split(std::string str, char delimeter);
};

