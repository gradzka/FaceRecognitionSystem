#include "Camera.h"

Camera::Camera(std::string IPAddress, std::string USERPWD)
{
	this->IPAddress = IPAddress;
	this->USERPWD = USERPWD;

	//create folder, where images will be stored
	std::experimental::filesystem::create_directory(this->IPAddress);
}

Camera::~Camera()
{
}

void Camera::captureFrame()
{
	cv::VideoCapture vcap;
	cv::Mat image;
	const std::string videoStreamAdress = "rtsp://"+ this->USERPWD +"@"+this->IPAddress+"/live/ch0";

	if (!vcap.open(videoStreamAdress))
	{
		printf("Error opening video stream or file\n");
		return;
	}

	std::time_t t = std::time(NULL);
	char FileName[20];
	std::string FileNameS;
	struct tm timeinfo;
	localtime_s(&timeinfo, &t);

	if (std::strftime(FileName, sizeof(FileName), "%F %H-%M-%S", &timeinfo))
	{
		if (vcap.read(image))
		{
			FileNameS = this->IPAddress + "/";
			FileNameS += std::string(FileName);
			FileNameS += ".png";
			cv::imwrite(FileNameS, image);			
		}
		else
		{
			printf("No frame captured!\n");
		}
	}
	else
	{
		printf("Problem with file creation!");
		return;
	}
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
		std::string URL = "http://" + IPAddress+ "/hy-cgi/ptz.cgi?cmd=ptzctrl&act=";
		switch (command)
		{
			case VK_UP: //gora
			{
				URL += "up";
				break;
			}
			case VK_DOWN: //dol
			{
				URL += "down";
				break;
			}
			case VK_LEFT: //lewo
			{
				URL += "left";
				break;
			}
			case VK_RIGHT: //prawo
			{
				URL += "right";
				break;
			}
			case 0x48: //pozycja domowa (H)
			{
				URL += "home";
				break;
			}
			case 0x50: //zdjecie (P)
			{
				printf("Type directory name where the pictures will be stored:\n");
				std::string dirName="";
				scanf_s("%s", dirName);
				printf("Type number of pictures that will be made:\n");
				int numberPictures=0;
				scanf_s("%d", numberPictures);
				captureFrame();
				return;
			}
			case 0: //zatrzymaj kamere
			{
				URL += "stop";
				break;
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
