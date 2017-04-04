#include "Camera.h"
#include "Utilities.h"

bool checkKeyboard(Camera *camera, int key)
{
	//SHORT x = 0;
	if (GetAsyncKeyState(key) & 0x8000) //change sign of number
	{
		//printf("%d UP!!!\n", key); // x & 0x8000 = 32768
		//x = GetAsyncKeyState(key);
		//printf("%d!!!\n", x & 0x8000);
		if (key == VK_ESCAPE)
		{
			return 1;
		}
		if (key == 0x43)
		{
			Utilities::printCommandList();
		}
		if (key == 0x50)
		{
			Utilities::cleanBuffer();
		}
		camera->sendMessage(key);

		while (GetAsyncKeyState(key) & 0x8000)
		{
			//x = GetAsyncKeyState(key);
		}
		if (key < 0x25 || key > 0x28)
		{
			return 0;
		}
		camera->sendMessage(0);
		//printf("%d DOWN!!!\n", key); // x & 0x8000 = 0
	}
	return 0;
}
void t_keyboard(Camera *camera)
{
	bool state = 0;
	while (!state)
	{
		checkKeyboard(camera, VK_UP);			//up
		checkKeyboard(camera, VK_DOWN);			//down
		checkKeyboard(camera, VK_LEFT);			//left
		checkKeyboard(camera, VK_RIGHT);		//right
		checkKeyboard(camera, 0x48);			//home posiotion (H)
		checkKeyboard(camera, 0x50);			//take picture (P)
		checkKeyboard(camera, 0x43);			//command list
		state= checkKeyboard(camera, VK_ESCAPE);//exit
	}
}
void printLogo()
{
	std::cout << "  ___" << std::endl;
	std::cout << " | __| __ _  __  ___" << std::endl;
	std::cout << " | _| / _` |/ _|/ -_)" << std::endl;
	std::cout << " |_|  \\__,_|\\__|\\___|" << std::endl;
	std::cout << "  ___                            _  _    _" << std::endl;
	std::cout << " | _ \\ ___  __  ___  __ _  _ _  (_)| |_ (_) ___  _ _" << std::endl;
	std::cout << " |   \/\/ -_)\/ _|\/ _ \\\/ _` || ' \\ | ||  _|| |\/ _ \\| ' \\ " << std::endl;
	std::cout << " |_|_\\\\___|\\__|\\___\/\\__, ||_||_||_| \\__||_|\\___\/|_||_|" << std::endl;
	std::cout << "  ___            _  |___\/" << std::endl;
	std::cout << " / __| _  _  ___| |_  ___  _ __ " << std::endl;
	std::cout << " \\__ \\| || |(_-<|  _|/ -_)| '  \\ " << std::endl;
	std::cout << " |___/ \\_, |/__/ \\__|\\___||_|_|_| " << std::endl;
	std::cout << "       |__/ " << std::endl << std::endl << std::endl;
}
int main(int argc, char *argv[])
{
	system("cls");

	printLogo();

	std::cout << "HELP - C" << std::endl;
	std::cout << "Type: ";

	
	Camera *camera = new Camera("IP", "login:password");
	if (argc<=1) //no parameters
	{
		t_keyboard(camera);
	}
	else //time parameter (in minutes)
	{
		//TODO
	}

	curl_global_cleanup();
	delete camera;
	return 0;
}

