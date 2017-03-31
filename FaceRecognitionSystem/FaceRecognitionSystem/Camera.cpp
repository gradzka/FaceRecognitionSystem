#include "Camera.h"

Camera::Camera(string IPAddress, string USERPWD)
{
	this->IPAddress = IPAddress;
	this->USERPWD = USERPWD;
}

Camera::~Camera()
{
}

void Camera::sendMessage(int command)
{
	curl = curl_easy_init();
	if (curl)
	{
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_USERPWD, USERPWD.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.28 Safari/537.36");
		string URL = "http://" + IPAddress;
		switch (command)
		{
			case VK_UP: //gora
			{
				URL += "/hy-cgi/ptz.cgi?cmd=ptzctrl&act=up";
				break;
			}
			case VK_DOWN: //dol
			{
				URL += "/hy-cgi/ptz.cgi?cmd=ptzctrl&act=down";
				break;
			}
			case VK_LEFT: //lewo
			{
				URL += "/hy-cgi/ptz.cgi?cmd=ptzctrl&act=left";
				break;
			}
			case VK_RIGHT: //prawo
			{
				URL += "/hy-cgi/ptz.cgi?cmd=ptzctrl&act=right";
				break;
			}
			case 0x48: //pozycja domowa (H)
			{
				URL += "/hy-cgi/ptz.cgi?cmd=ptzctrl&act=home";
				break;
			}
			case 0x50: //zdjecie (P)
			{

			}
			case 0: //zatrzymaj kamere
			{
				URL += "/hy-cgi/ptz.cgi?cmd=ptzctrl&act=stop";
			}
			default:
			{
				curl_easy_cleanup(curl);
				return;
			}
		}
		curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());

		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}
