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
	Utilities::dashes();
	std::cout << "<BUTTON>\t<Description>" << std::endl;
	std::cout << "W\t\tMove the camera up" << std::endl;
	std::cout << "S\t\tMove the camera down" << std::endl;
	std::cout << "A\t\tMove the camera left" << std::endl;
	std::cout << "D\t\tMove the camera right" << std::endl;
	std::cout << "H\t\tMove the camera to home position" << std::endl;;
	std::cout << "P\t\tTake pictures" << std::endl;
	std::cout << "E\t\tCount people" << std::endl;
	std::cout << "T\t\tTrain face recognizer" << std::endl;
	std::cout << "R\t\tStart face recognizer" << std::endl;
	std::cout << "C\t\tShow command list" << std::endl;
	std::cout << "Q\t\tExit the application" << std::endl;
	Utilities::dashes();
	std::cout << "Type: ";
}
void Utilities::dashes()
{
	std::cout << "--------------------------------------------------------------------" << std::endl;
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
void Utilities::maskPassword(std::string &password)
{
	char c = ' ';
	while (true)
	{
		c = _getch();
		if (c == 13) { break; } //13 - ENTER
		else if (c == 8)
		{
			password.pop_back();
		}
		else
		{
			password.push_back(c);
		}
	}
	std::cout << std::endl;
	dashes();
}

std::vector<std::string> Utilities::split(std::string str, char delimeter)
{
	std::vector<std::string> internal;
	std::stringstream ss(str);
	std::string tok;

	while (std::getline(ss, tok, delimeter))
	{
		internal.push_back(tok);
	}

	return internal;
}

bool Utilities::SaveToBinFile(std::string fileName, BYTE *data)
{
	FILE *file=NULL;
	fopen_s(&file, fileName.c_str(), "wb");
	if (file != NULL)
	{
		fwrite(data, 1, 32, file);
	}
	else
	{
		std::cout << fileName << " is corrupted!" << std::endl;
		fclose(file);
		return false;
	}

	fclose(file);
	return true;
}
BYTE * Utilities::ReadFromBinFile(std::string fileName)
{
	FILE *file=NULL;
	fopen_s(&file, fileName.c_str(), "rb");
	if (file != NULL)
	{
		long lSize;
		BYTE * data;
		size_t result;
		
		fseek(file, 0, SEEK_END);
		lSize = ftell(file);
		rewind(file);

		data = new BYTE[lSize];
		if (data!=NULL)
		{
			result = fread(data, 1, lSize, file);

			if (result == lSize)
			{
				fclose(file);
				return data;
			}
			else
			{
				fclose(file);
				return NULL;
			}
			
		}
	}
	else
	{
		std::cout << fileName << " is corrupted!" << std::endl;
		fclose(file);
		return NULL;
	}	
}
int Utilities::BinFileElementsNo(std::string fileName)
{
	FILE *file = NULL;
	int lSize = 0;
	fopen_s(&file, fileName.c_str(), "rb");
	if (file != NULL)
	{
		
		BYTE * data;
		size_t result;

		fseek(file, 0, SEEK_END);
		lSize = ftell(file);
	}

	fclose(file);
	return lSize;
}

void Utilities::PrintEnd()
{
	dashes();
	std::cout << "HELP - C" << std::endl;
	dashes();
	std::cout << "Type: ";
}



