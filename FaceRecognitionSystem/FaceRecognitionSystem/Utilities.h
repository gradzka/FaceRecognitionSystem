#pragma once
#include <conio.h>
#include <iostream>
#include <climits>
#include <regex>
#include <sstream>
#include <vector>

typedef unsigned char BYTE;// 8-bit byte

class Utilities
{
public:
	Utilities();
	~Utilities();
	static void printCommandList();
	static void dashes();
	static void cleanBuffer();
	static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
	static void maskPassword(std::string &password);
	static std::vector<std::string> split(std::string str, char delimeter);
	static bool SaveToBinFile(std::string fileName, BYTE *data);
	static BYTE *ReadFromBinFile(std::string fileName);
	static int BinFileElementsNo(std::string fileName);
};

