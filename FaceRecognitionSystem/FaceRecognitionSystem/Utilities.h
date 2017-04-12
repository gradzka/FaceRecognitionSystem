#pragma once
#include <conio.h>
#include <iostream>
#include <climits>

class Utilities
{
public:
	Utilities();
	~Utilities();
	static void printCommandList();
	static void cleanBuffer();
	static size_t Utilities::write_data(void *buffer, size_t size, size_t nmemb, void *userp);
};

