#include "Camera.h"
#include "conio.h"

void printCommandList()
{
	printf("BUTTON\tDescription\n");
	printf("UP\tMove the camera up\n");
	printf("DOWN\tMove the camera down\n");
	printf("LEFT\tMove the camera left\n");
	printf("RIGHT\tMove the camera right\n");
	printf("H\tMove the camera to home position\n");
	printf("P\tTake pictures\n");
	printf("C\tShow command list\n");
	printf("ESC\tExit the application\n\n");
}
void cleanBuffer()
{
	unsigned char a = 0;
	if (_kbhit()) {
		a = _getch();
		while (_kbhit())
			_getch();
	}
}
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
			printCommandList();
		}
		if (key == 0x50)
		{
			cleanBuffer();
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
	printf("Keyboard!!!\n");
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
		state=checkKeyboard(camera, VK_ESCAPE);	//exit
	}
}

int main(int argc, char *argv[])
{
	Camera *camera = new Camera("IP", "Login:Password");
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

