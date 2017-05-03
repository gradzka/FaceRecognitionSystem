#include "ImgProc.h"



ImgProc::ImgProc()
{
	//face_cascade_name = "haarcascade_frontalface_alt.xml";
	face_cascade_name = "lbpcascade_frontalface.xml";
	window_name = "Hello Face!";
	model = cv::createEigenFaceRecognizer();
}


ImgProc::~ImgProc()
{ }

int ImgProc::detectFace(cv::Mat img)
{
	std::vector<cv::Rect> faces;
	cv::Mat img_gray;

	cvtColor(img, img_gray, CV_BGR2GRAY);
	cv::equalizeHist(img_gray, img_gray);
	//const int scale = 2;

	face_cascade.detectMultiScale(img_gray, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
	/*int percentheigth = 0;
	int percentwidth = 0;
	for (unsigned i = 0; i < faces.size(); i++)
	{
		/*percentheigth = 0.4*faces[i].height;
		faces[i].height *= 2;
		faces[i].y *= 2;

		percentwidth = 0.4*faces[i].width;
		faces[i].width *= 2;
		faces[i].x *= 2;

		cv::Rect rect_face(faces[i]);
		rectangle(img, rect_face, cv::Scalar(120, 5, 86), 2, 2, 0);
	}*/
	//imshow(window_name, img);

	return faces.size();
}

bool ImgProc::createCSV()
{
	std::fstream file;
	file.open("corp.csv", std::ios::out);
	if (file.good() != true)
	{
		std::cout << "Error!" << std::endl;
		return false;
	}

	std::string path = "screenshots/";
	int numberPerson = -1;
	for (auto & p : std::experimental::filesystem::recursive_directory_iterator(path))
	{
		if (!std::experimental::filesystem::is_directory(p.path()))
		{
			file << p << ";" << numberPerson << std::endl;
		}
		else
		{
			numberPerson++;
		}
	}
	file.close();
	return true;
}

void ImgProc::countPeople(std::string userPwd, std::string addressIP)
{
	std::cout<<std::endl << "wait..." << std::endl;
	cv::VideoCapture vcap;
	cv::Mat image;
	const std::string videoStreamAdress = "rtsp://"+userPwd+"@"+addressIP+":80/live/ch1";

	if (!vcap.open(videoStreamAdress))
	{
		std::cout << "Error opening video stream or file." << std::endl;
		return;
	}
	if (face_cascade.load(face_cascade_name))
	{
		int peopleNo = 0;
		for (int i=0;i<10;i++)
		{
			if (vcap.read(image))
			{
				peopleNo += detectFace(image);
				cv::waitKey(1);
			}
		}
		std::cout << "Number of detected people: " << round((double)peopleNo / 10) << "." << std::endl << std::endl;
	}
	else
	{
		std::cout << face_cascade_name + " not found." << std::endl;
	}
	std::cout << "HELP - C" << std::endl;
	std::cout << "Type: ";
}
void ImgProc::read_csv(const std::string & filename, std::vector<cv::Mat> & images, std::vector<int> & labels)
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file)
	{
		std::cout << "Open " << filename << " file failed.";
		return;
	}
	std::string line, path, classlabel;
	while (getline(file, line))
	{
		std::stringstream liness(line);
		getline(liness, path, ';');
		getline(liness, classlabel);
		//classlabel.erase(0, 1);
		//path.erase(path.length() - 1, path.length());
		if (!path.empty() && !classlabel.empty())
		{
			images.push_back(cv::imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
			//std::cout << path << "=" << atoi(classlabel.c_str()) << std::endl;
		}
	}
}
void ImgProc::TrainFaceRecognizer()
{
	Utilities::cleanBuffer();
	char yesNo;
	if (std::experimental::filesystem::exists("trainFR.xml") && !std::experimental::filesystem::is_empty("trainFR.xml"))
	{
		std::cout << std::endl<< "Do you want to load training data from trainFR.xml file?: Yes[Y]/No[N]\n";
		yesNo = (char)getchar();
		while ((yesNo != 'N' && yesNo != 'n') && (yesNo != 'Y' && yesNo != 'y'))
		{
			std::cout << "\nType: Yes[Y]/No[N]\n";
			yesNo = (char)getchar();
		}		
		if (yesNo == 'Y' || yesNo == 'y')
		{
			std::cout << "wait..." << std::endl;
			model->load("trainFR.xml");
			std::cout << "Face recognizer training data load successfully!" << std::endl;
			std::cout <<std::endl<< "HELP - C" << std::endl;
			std::cout << "Type:";
			return;
		}
	}

	std::cout << "wait..." << std::endl;
	createCSV();
	read_csv("corp.csv", images, labels);
	model->train(images, labels);
	std::cout << "Face recognizer has trained successfully!" << std::endl;

	std::cout << "Do you want to save training data in trainFR.xml file?: Yes[Y]/No[N]\n";
	std::cin.ignore();
	yesNo = (char)getchar();
	while ((yesNo != 'N' && yesNo != 'n') && (yesNo != 'Y' && yesNo != 'y'))
	{
		std::cout << "\nType: Yes[Y]/No[N]\n";
		yesNo = (char)getchar();
	}
	if (yesNo == 'Y' || yesNo == 'y')
	{
		std::cout << std::endl << "wait..." << std::endl;
		model->save("trainFR.xml");
		std::cout << "Training data has saved successfully!" << std::endl;
	}
	std::cout << std::endl<< "HELP - C" << std::endl;
	std::cout << "Type:";
}
void ImgProc::predictPerson(std::string userPwd, std::string addressIP)
{
	if (model->getLabelInfo(0)=="")
	{
		std::cout << "Error! Model is not trained!" << std::endl;
		return;
	}
	std::cout << std::endl << "wait..." << std::endl;
	cv::VideoCapture vcap;
	cv::Mat image;
	const std::string videoStreamAdress = "rtsp://" + userPwd + "@" + addressIP + ":80/live/ch1";

	if (!vcap.open(videoStreamAdress))
	{
		std::cout << "Error opening video stream or file." << std::endl;
		return;
	}
	if (face_cascade.load(face_cascade_name))
	{
		std::vector<cv::Rect> faces;
		cv::Mat img_gray;
		cv::Mat img_resized;
		cv::Mat img_cut;
		cv::namedWindow("Sample", CV_WINDOW_AUTOSIZE);
		cv::namedWindow("Predicted", CV_WINDOW_AUTOSIZE);
		int plabel=-1;
		double predicted_confidence = 0.0;
		while(true)
		{
			if (vcap.read(image))
			{
				cvtColor(image, img_gray, CV_BGR2GRAY);
				cv::equalizeHist(img_gray, img_gray);

				face_cascade.detectMultiScale(img_gray, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
				for (unsigned i = 0; i < faces.size(); i++)
				{
					img_cut = image(faces[i]);
					resize(img_cut, img_resized, cv::Size(200, 200), 1.0, 1.0, cv::INTER_CUBIC);
					model->predict(img_gray, plabel, predicted_confidence);
					std::cout << plabel;
					std::cout << "Predicted person: " << plabel << " with confidence: " << predicted_confidence<< "." << std::endl;
				}
				cv::waitKey(1);
			}
		}
		std::cout << std::endl;

	}
	else
	{
		std::cout << face_cascade_name + " not found." << std::endl;
	}

}
