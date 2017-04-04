#include "Camera.h"

Camera::Camera(std::string IPAddress, std::string USERPWD)
{
	this->IPAddress = IPAddress;
	this->USERPWD = USERPWD;
}

Camera::~Camera()
{
}
int Camera::empty_stream(std::istream & is) {

	std::streambuf * pbuf = std::cin.rdbuf();
	std::streamsize size = pbuf->in_avail();

	if (size)
	{
		if (!is.good())
			return 0;
		is.sync();
		return 1;
	}
	else
		return -1;
}
void Camera::captureFrame()
{
	std::string dirName;
	std::cout << std::endl;
	std::cout << "Type directory name where the pictures will be stored:" << std::endl;

	Utilities::cleanBuffer();
	std::cin.ignore();
	if (std::cin.good()==false || std::cin.eof())
	{
		std::cin.clear();
		std::cin.ignore(MAXINT, '\n');
	}

	
	std::getline(std::cin, dirName);	
	std::cout << "Type number of pictures that will be made:" << std::endl;
	int numberOfPictures = 0;
	std::cin >> numberOfPictures;
	std::cout << "wait..." << std::endl;

	if (numberOfPictures > 0) {

		//create folder, where images will be stored
		std::experimental::filesystem::create_directory("screenshots/" + dirName);

		cv::VideoCapture vcap;
		cv::Mat image;
		const std::string videoStreamAdress = "rtsp://" + this->USERPWD + "@" + this->IPAddress + "/live/ch0";

		if (!vcap.open(videoStreamAdress))
		{
			std::cout << "Error opening video stream or file" << std::endl;
			return;
		}

		std::time_t time;
		char FileName[20];
		std::string FileNameS;
		struct tm timeinfo;

		do {
			time = std::time(NULL);
			localtime_s(&timeinfo, &time);
			if (std::strftime(FileName, sizeof(FileName), "%F %H-%M-%S", &timeinfo))
			{
				if (vcap.read(image))
				{
					FileNameS = "screenshots/" + dirName + "/";
					FileNameS += std::string(FileName);
					FileNameS += ".png";
					cv::imwrite(FileNameS, image);
				}
				else
				{
					std::cout << "No frame captured!" << std::endl;
					return;
				}
				numberOfPictures--;
			}
			else
			{
				std::cout << "Problem with file creation!" << std::endl;
				return;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		} while (numberOfPictures > 0);
		std::cout << "Success! Your screenshot/s in dir " << dirName << "." << std::endl;	
	}
	else
	{
		std::cout << "Error! No screenshots were taken."<<std::endl;
	}
	std::cout << "Type:";
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
