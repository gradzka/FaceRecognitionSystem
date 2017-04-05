#pragma once
#include "AES256/aes256.hpp"
#include <conio.h>
#include <iostream>

class Utilities
{
public:
	Utilities();
	~Utilities();
	static void printCommandList();
	static void cleanBuffer();
};

