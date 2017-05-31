#include "Camera.h"
#include <regex>
#include "AES.h"

bool checkKeyboard(Camera *camera, char key)
{

	//checkKeyboard(camera, VK_UP);			//up
	//checkKeyboard(camera, VK_DOWN);			//down
	//checkKeyboard(camera, VK_LEFT);			//left
	//checkKeyboard(camera, VK_RIGHT);		//right
	//checkKeyboard(camera, VK_HOME);			//home posiotion (H)
	//checkKeyboard(camera, VK_PICTURE);		//take picture (P)
	//checkKeyboard(camera, VK_COMMAND);		//command list (C)
	//checkKeyboard(camera, VK_ATTENDANCE);	//count people (A)
	//checkKeyboard(camera, VK_TRAIN_FR);		//train FaceRecognizer (T)
	//checkKeyboard(camera, VK_FR);			//Recognize face (R)
	//state=checkKeyboard(camera, VK_ESCAPE);	//exit	

	if (key == VK_EXIT) { std::cout << key; return 1; }
	if (key == VK_COMMAND) { std::cout << key; Utilities::printCommandList(); }
	if (key == VK_PICTURE) { std::cout << key; Utilities::cleanBuffer(); }
	if (key==VK_ESTIMATE || key == VK_TRAIN_FR){std::cout << key; }
	camera->sendMessage(key);

	
	
	//if (key < VK_LEFT || key > VK_DOWN) { return 0; }
	if (key == 'w' || key == 's' || key == 'a' || key == 'd') 
	{
		/*int button = 0;
		if (key == 'w') { button = 0x26; }
		else if (key == 's') { button = 0x28; }
		else if (key == 'a') { button = 0x25; }
		else if (key == 'd') { button = 0x27; }
		while (GetAsyncKeyState(button) & 0x8000) { }*/
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		camera->sendMessage(0);
	}

	return 0;
}
void t_keyboard(Camera *camera)
{
	bool state = 0;
	char button = ' ';
	while (!state)
	{		
		button = _getch();
		state=checkKeyboard(camera, button);
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
	Utilities::dashes();
}
Camera* cameraConfiguration()
{
	std::cout << std::endl << "      *** Connection configuration with IP camera ***" << std::endl << std::endl;
	Utilities::dashes();
	std::cout << "Type:" << std::endl;
	std::cout << "1 - manual configuration" << std::endl;
	std::cout << "2 - load configuration from config file" << std::endl;
	std::cout << "3 - create config file" << std::endl;
	Utilities::dashes();
	std::string choice = "";

	do {
		std::cout << "Type: ";
		std::cin >> choice;
		Utilities::dashes();
	} while (choice!="1" && choice != "2" && choice != "3");

	Camera* camera = NULL;

	switch ((int)choice[0])
	{
		case '1':
		{
			camera = camera->typeCameraData();
			break;
		}
		case '2':
		{
			AES *aes = new AES();

			std::string key = "";
			std::cout << "Type secret key: ";
			Utilities::maskPassword(key);

			std::string secretData = aes->aes_decrypt(key);
			//secretData = camera->getIPaddress() + "," + camera->getUSERPWD();
			//sep[0] camera->getIPaddress()
			//sep[1] camera->getUSERPWD()
			std::vector<std::string> sep = Utilities::split(secretData, ',');
			if (sep.size() == 2)
			{
				camera = new Camera(sep[0], sep[1]);
			}
			else
			{
				std::cout << "Config file is missed or corrupted!\n\n";
				camera = NULL;
				Utilities::dashes();
			}

			if (camera!=NULL)
			{
				camera->testConnection(camera);
			}
			break;
		}
		case '3':
		{
			camera = camera->typeCameraData();
			if (camera != NULL)
			{
				AES *aes = new AES();
				std::string secretData = camera->getIPaddress() + "," + camera->getUSERPWD();
				std::cout << "Type key: ";
				std::string password = "";
				Utilities::maskPassword(password);
				aes->aes_encrypt(secretData, password);
				std::cout << "Your data was encrypted with AES algorithm and saved in config.bin" << std::endl;
				Utilities::dashes();
			}
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
		camera = cameraConfiguration();
	} while (camera == NULL);

	//Camera * camera = new Camera("IP","login:password");
	std::cout << std::endl << "      ***    Welcome in Face Recognition System    ***" << std::endl << std::endl;
	Utilities::PrintEnd();
	

	t_keyboard(camera);
	

	curl_global_cleanup();
	delete camera;
	return 0;
}

