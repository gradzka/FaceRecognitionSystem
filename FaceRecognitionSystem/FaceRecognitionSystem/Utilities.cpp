#include "Utilities.h"



Utilities::Utilities()
{
}


Utilities::~Utilities()
{
}

void Utilities::printCommandList()
{
	std::cout << std::endl;
	std::cout << "BUTTON\tDescription" << std::endl;
	std::cout << "UP\tMove the camera up" << std::endl;
	std::cout << "DOWN\tMove the camera down" << std::endl;
	std::cout << "LEFT\tMove the camera left" << std::endl;
	std::cout << "RIGHT\tMove the camera right" << std::endl;
	std::cout << "H\tMove the camera to home position" << std::endl;;
	std::cout << "P\tTake pictures" << std::endl;
	std::cout << "C\tShow command list" << std::endl;
	std::cout << "ESC\tExit the application" << std::endl;
	std::cout << "Type: ";
}
void Utilities::cleanBuffer()
{
	unsigned char a = 0;
	if (_kbhit()) {
		a = _getch();
		while (_kbhit())
			_getch();
	}
	std::cin.ignore();
	if (std::cin.good()==false || std::cin.eof())
	{
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
	}
}
size_t Utilities::write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	return size * nmemb;
}