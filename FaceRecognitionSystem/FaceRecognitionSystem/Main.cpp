#include "Camera.h"

void checkKeyboard(Camera *camera, int key)
{
	//SHORT x = 0;
	if (GetAsyncKeyState(key) & 0x8000) //change sign of number
	{
		//printf("%d UP!!!\n", key); // x & 0x8000 = 32768
		//x = GetAsyncKeyState(key);
		//printf("%d!!!\n", x & 0x8000);
		if (key == VK_ESCAPE)
		{
			exit(0);
		}
		camera->sendMessage(key);
		if (key < 0x25 || key > 0x28)
		{
			return;
		}
		while (GetAsyncKeyState(key) & 0x8000)
		{
			//x = GetAsyncKeyState(key);
		}
		camera->sendMessage(0);
		//printf("%d DOWN!!!\n", key); // x & 0x8000 = 0
	}
}

void t_keyboard(Camera *camera)
{
	printf("Keyboard!!!\n");
	while (1)
	{
		checkKeyboard(camera, VK_UP);		//gora
		checkKeyboard(camera, VK_DOWN);		//dol
		checkKeyboard(camera, VK_LEFT);		//lewo
		checkKeyboard(camera, VK_RIGHT);	//prawo
		checkKeyboard(camera, 0x48);		//pozycja domowa (H)
		checkKeyboard(camera, 0x50);		//zdjecie (P)
		checkKeyboard(camera, VK_ESCAPE);	//wyjscie
	}
}

int main()
{
	Camera *camera = new Camera("IP", "Login:Password");
	t_keyboard(camera);

	curl_global_cleanup();
	delete camera;
	return 0;
}

