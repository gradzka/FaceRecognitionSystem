#include "Camera.h"
#include <regex>

bool checkKeyboard(Camera *camera, int key)
{
	if (GetAsyncKeyState(key) & 0x8000) //change sign of number
	{
		if (key == VK_ESCAPE) {	return 1; }
		if (key == VK_COMMAND) { Utilities::printCommandList(); }
		if (key == VK_PICTURE) { Utilities::cleanBuffer(); }
		camera->sendMessage(key);

		while (GetAsyncKeyState(key) & 0x8000) { }
		if (key < VK_LEFT || key > VK_DOWN) { return 0; }
		camera->sendMessage(0);
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
		checkKeyboard(camera, VK_HOME);			//home posiotion (H)
		checkKeyboard(camera, VK_PICTURE);		//take picture (P)
		checkKeyboard(camera, VK_COMMAND);		//command list
		state=checkKeyboard(camera, VK_ESCAPE);	//exit
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
Camera* cameraConfiguration()
{
	std::cout << "*** Connection configuration with IP camera. ***" << std::endl << std::endl;
	std::cout << "Type:" << std::endl;
	std::cout << "1 - manual configuration," << std::endl;
	std::cout << "2 - make config file," << std::endl;
	std::cout << "3 - file configuration." << std::endl;
	int choice = 0;

	do {
		std::cout << "Choice: ";
		std::cin >> choice;
	} while (choice<=0 || choice>3);

	Camera* camera = NULL;

	switch (choice)
	{
		case 1:
		{
			camera->typeCameraData(); //if null test connection failed
			break;
		}
		case 2:
		{
			break;

		}
		case 3: //creation and setup TODO
		{
			break;

		}
		default:
		{
			break;
		}
	}

	return camera;

}
int main(int argc, char *argv[])
{
	system("cls");

	printLogo();
	Camera *camera;
	do
	{
		camera = cameraConfiguration(); //TODO
	} while (camera == NULL);

	//Camera * camera = new Camera("IP","login:password");
	
	std::cout << "HELP - C" << std::endl;
	std::cout << "Type: ";
	
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

