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

void Camera::captureFrame()
{
	std::string dirName;
	std::cout << std::endl;
	std::cout << "Type directory name where the pictures will be stored:" << std::endl;

	//Utilities::cleanBuffer();
	
	std::getline(std::cin, dirName);	
	std::cout << "Type number of pictures that will be made:" << std::endl;
	int numberOfPictures = 0;
	std::cin >> numberOfPictures;
	std::cout << "wait..." << std::endl;

	if (numberOfPictures > 0) {
		std::experimental::filesystem::create_directory("screenshots/");
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
			case VK_HOME: //pozycja domowa (H)
			{
				URL += "home";
				break;
			}
			case VK_PICTURE: //zdjecie (P)
			{
				captureFrame();				
				return;
			}
			case VK_ATTENDANCE: //zlicz liczbe osob (A)
			{
				imgproc->countPeople(this->USERPWD, this->IPAddress);
				return;
			}
			case VK_TRAIN_FR: //trenuj FaceRecognizer (T)
			{
				std::cout << std::endl<< "wait..." << std::endl;
				imgproc->createCSV();
				std::vector<cv::Mat> images;
				std::vector<int> labels;
				imgproc->read_csv("corp.csv", images, labels);
				cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
				model->train(images, labels);
				//model->save("TUTAJ.txt"); -TODO

				/*cv::Mat test = cv::imread("test3.pgm", 0); //0 -gray picture, >0 BGR format
				if (!test.data)
				{
					std::cout << "There's no test3.pgm" << std::endl;
					return;
				}
				cv::namedWindow("Sample", CV_WINDOW_AUTOSIZE);
				cv::namedWindow("Predicted", CV_WINDOW_AUTOSIZE);
				int plabel = model->predict(test);
				std::cout << plabel;
				cv::imshow("Sample", test);
				int i = 0;
				cv::imshow("Predicted", images[5 * plabel + (i % 5)]);
				while (cv::waitKey(0) != 27)
				{
					i++;
					cv::imshow("Predicted", images[5 * plabel + (i % 5)]);
				}*/
				std::cout << "Face recognizer has trained successfully!" << std::endl;
				std::cout << "Type:";
				return;
			}
			case VK_FR: //rozpocznij rozpoznawanie twarzy (R)
			{
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
	std::cout << "Test connection." << std::endl;
	std::cout << "wait..." << std::endl;
	if (camera->testConnectionCurl() == true)
	{
		std::cout << "Test connection succeeded." << std::endl << std::endl;
	}
	else
	{
		camera = NULL;
		std::cout << "Test connection failed." << std::endl << std::endl;
	}
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
		std::cout << "Type camera adress IP" << std::endl;
		std::cout << "compatible with format: xxx.xxx.xxx.xxx" << std::endl;
		std::cin >> cameraIP;
	} while (std::regex_match(cameraIP, cameraIPpattern) == false);

	std::cout << "Type login" << std::endl;
	std::cin >> login;

	std::cout << "Type password" << std::endl;
	Utilities::maskPassword(password);

	loginPassword = login + ":" + password;

	camera = new Camera(cameraIP, loginPassword);

	testConnection(camera);
	

	return camera;
}
