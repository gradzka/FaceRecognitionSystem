#pragma once
#include "Camera.h"
#include "AES256/aes256.hpp"
#include <conio.h>

class Utilities
{
public:
	Utilities();
	~Utilities();
	static void printCommandList();
	static void cleanBuffer();
};

