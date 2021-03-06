#include "Camera.h"

Camera::Camera(std::string IPAddress, std::string USERPWD)
{
	this->IPAddress = IPAddress;
	this->USERPWD = USERPWD;
	imgproc = new ImgProc();
}

Camera::~Camera()
{
}

double Camera::getFPS()
{
	std::cout << "Configuration of IP camera settings. Wait... " << std::endl;
	Utilities::dashes();
	cv::VideoCapture vcap;
	cv::Mat image;
	const std::string videoStreamAdress = "rtsp://" + this->USERPWD + "@" + this->IPAddress + "/live/ch1";

	if (!vcap.open(videoStreamAdress))
	{
		std::cout << "Error opening video stream or file" << std::endl;
		return 0;
	}
	int num_frames = 100;
	time_t start, end;
	time(&start);
	for (size_t i = 0; i < num_frames; i++)
	{
		vcap >> image;
	}
	time(&end);

	double seconds=difftime(end, start);
	vcap.release();
	return num_frames / seconds;
}
void Camera::CaptureFrame()
{
	Utilities::dashes();
	std::string dirName;
	std::cout << "Type directory name where pictures will be stored" << std::endl;
	std::cout << "Type: ";

	//Utilities::cleanBuffer();
	
	std::getline(std::cin, dirName);
	Utilities::dashes();
	std::cout << "Type number of pictures that will be made" << std::endl;
	std::cout << "Type: ";
	std::cout << std::endl;
	Utilities::dashes();
	int numberOfPictures = 0;
	std::cin >> numberOfPictures;
	std::cout << std::endl;
	Utilities::dashes();

	if (numberOfPictures > 0) {
		std::experimental::filesystem::create_directory("screenshots/");
		//create folder, where images will be stored
		std::experimental::filesystem::create_directory("screenshots/" + dirName);

		double fps = getFPS(); //number of frame per second
		if (fps == 0) {
			std::cout << "Error opening video stream" << std::endl;
			return;
		}

		std::time_t time;
		char FileName[20];
		std::string FileNameS;
		struct tm timeinfo;

		int frameID = 0;
		int frequency = 3;
		Beep(1568, 1000);
		cv::VideoCapture vcap;
		cv::Mat image;

		std::vector<cv::Rect> faces;
		cv::Mat img_gray;                                 
		cv::Mat img_cut;
		cv::Mat img_resized;
		cv::Mat resized_gray;

		const std::string videoStreamAdress = "rtsp://" + this->USERPWD + "@" + this->IPAddress + "/live/ch0";

		if (!vcap.open(videoStreamAdress))
		{
			std::cout << "Error opening video stream or file" << std::endl;
			return;
		}
		
		do {
			frameID = vcap.get(CV_CAP_PROP_POS_FRAMES);//current frame number
			time = std::time(NULL);
			localtime_s(&timeinfo, &time);
			if (std::strftime(FileName, sizeof(FileName), "%F %H-%M-%S", &timeinfo))
			{
				if (vcap.read(image))
				{
					if (frameID % (int)fps*frequency == 0 && frameID!=0)
					{
						Beep(1568, 200);
						FileNameS = "screenshots/" + dirName + "/";
						FileNameS += std::string(FileName);
						FileNameS += ".png";
						cv::imwrite(FileNameS, image);
						numberOfPictures--;						
					}				
				}
				else
				{
					std::cout << "No frame captured!" << std::endl;
					return;
				}			
			}
			else
			{
				std::cout << "Problem with file creation!" << std::endl;
				return;
			}
			
			//std::this_thread::sleep_for(std::chrono::milliseconds(1000));		
		} while (numberOfPictures > 0);
		Beep(1568, 1000);
		std::cout << "Success! Your screenshot/s in dir " << dirName << "." << std::endl;
		Utilities::dashes();
	}
	else
	{
		std::cout << "Error! No screenshots were taken."<<std::endl;
	}
	std::cout << "Type:";
}
void Camera::CaptureFrameToCorp()
{
	std::string dirName;
	std::cout << std::endl;
	Utilities::dashes();
	std::cout << "Type directory name where the pictures will be stored" << std::endl;
	std::cout << "Type: ";

	//Utilities::cleanBuffer();
	std::getline(std::cin, dirName);
	Utilities::dashes();
	std::cout << "Type number of pictures that will be made" << std::endl;
	std::cout << "Type: ";
	int numberOfPictures = 0;
	std::cin >> numberOfPictures;
	Utilities::dashes();
	if (numberOfPictures > 0) {
		std::experimental::filesystem::create_directory("screenshots/");
		//create folder, where images will be stored
		std::experimental::filesystem::create_directory("screenshots/" + dirName);

		double fps = getFPS(); //number of frame per second
		if (fps == 0) {
			std::cout << "Error opening video stream" << std::endl;
			Utilities::dashes();
			return;
		}

		std::time_t time;
		char FileName[20];
		std::string FileNameS;
		struct tm timeinfo;

		int frameID = 0;
		int frequency = 3;
		Beep(1568, 1000);
		cv::VideoCapture vcap;
		cv::Mat image;
		cv::Mat croppedImage;
		const std::string videoStreamAdress = "rtsp://" + this->USERPWD + "@" + this->IPAddress + "/live/ch0";

		if (!vcap.open(videoStreamAdress))
		{
			std::cout << "Error opening video stream or file" << std::endl;
			Utilities::dashes();
			return;
		}
		//vcap.set(CV_CAP_PROP_FPS, fps);	
		std::cout << "Screenshots are being taking..." << std::endl;
		Utilities::dashes();
		do {
			frameID = vcap.get(CV_CAP_PROP_POS_FRAMES);//current frame number
			time = std::time(NULL);
			localtime_s(&timeinfo, &time);
			if (std::strftime(FileName, sizeof(FileName), "%F %H-%M-%S", &timeinfo))
			{
				if (vcap.read(image))
				{
					if (frameID % (int)fps*frequency == 0 && frameID != 0)
					{
						//przytnij
						croppedImage=imgproc->cropFace(image);
						if (croppedImage.channels() != image.channels())
						{
							Beep(1568, 200);
							FileNameS = "screenshots/" + dirName + "/";
							FileNameS += std::string(FileName);
							FileNameS += ".png";

							cv::imwrite(FileNameS, croppedImage);
							numberOfPictures--;
						}
					}
				}
				else
				{
					std::cout << "No frame captured!" << std::endl;
					Utilities::dashes();
					return;
				}
			}
			else
			{
				std::cout << "Problem with file creation!" << std::endl;
				Utilities::dashes();
				return;
			}

			//std::this_thread::sleep_for(std::chrono::milliseconds(1000));		
		} while (numberOfPictures > 0);
		Beep(1568, 1000);
		std::cout << "Success! Your screenshot/s in dir " << dirName<< std::endl;
		Utilities::dashes();
	}
	else
	{
		std::cout << "Error! No screenshots were taken." << std::endl;
		Utilities::dashes();
	}
	std::cout << "Type: ";
}
void Camera::sendMessage(char command)
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
			case VK_U: //gora
			{
				URL += "up";
				break;
			}
			case VK_D: //dol
			{
				URL += "down";
				break;
			}
			case VK_L: //lewo
			{
				URL += "left";
				break;
			}
			case VK_R: //prawo
			{
				URL += "right";
				break;
			}
			case VK_HOME: //pozycja domowa (H)
			{
				URL += "home";
				break;
			}
			case VK_PICTURE: //zdjecie (P)
			{
				CaptureFrameToCorp();				
				return;
			}
			case VK_ESTIMATE: //zlicz liczbe osob (A)
			{
				imgproc->countPeople(this->USERPWD, this->IPAddress);
				return;
			}
			case VK_TRAIN_FR: //trenuj FaceRecognizer (T)
			{
				imgproc->TrainFaceRecognizer();
				return;
			}
			case VK_FR: //rozpocznij rozpoznawanie twarzy (R)
			{
				imgproc->predictPerson(this->USERPWD, this->IPAddress);
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

bool Camera::testConnectionCurl()
{
	curl = curl_easy_init();
	if (curl)
	{
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_USERPWD, USERPWD.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.28 Safari/537.36");
		std::string URL = "http://" + IPAddress;
		curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Utilities::write_data);
		res = curl_easy_perform(curl);
		if (res == 0)
		{
			int httpCode = 0;
			res = curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &httpCode);
			if (httpCode == 200)
			{
				curl_easy_cleanup(curl);
				return true;
			}
		}
		curl_easy_cleanup(curl);
	}
	return false;

}
void Camera::testConnection(Camera *& camera) 
{
	std::cout << "Test connection. Wait... " << std::endl;
	Utilities::dashes();
	if (camera->testConnectionCurl() == true)
	{
		std::cout << "Test connection has been succeeded" << std::endl;
	}
	else
	{
		camera = NULL;
		std::cout << "Test connection has been failed" << std::endl;
	}
	Utilities::dashes();
}
Camera* Camera::typeCameraData()
{
	Camera *camera = NULL;
	std::string cameraIP = "";
	std::string login = "";
	std::string password = "";
	std::string loginPassword = "";
	std::regex cameraIPpattern("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");

	do {
		std::cout << "Type camera adress IP compatible with format: xxx.xxx.xxx.xxx" << std::endl;
		std::cout << "Type: ";
		std::cin >> cameraIP;
		Utilities::dashes();
	} while (std::regex_match(cameraIP, cameraIPpattern) == false);

	std::cout << "Type login: ";
	std::cin >> login;
	Utilities::dashes();

	std::cout << "Type password: ";
	Utilities::maskPassword(password);

	loginPassword = login + ":" + password;

	camera = new Camera(cameraIP, loginPassword);

	testConnection(camera);
	

	return camera;
}
