#include "ImgProc.h"



ImgProc::ImgProc()
{
	//face_cascade_name = "haarcascade_frontalface_alt.xml";
	face_cascade_name = "lbpcascade_frontalface.xml";
	window_name = "Hello Face!";
}


ImgProc::~ImgProc()
{ }

int ImgProc::detectFace(cv::Mat img)
{
	std::vector<cv::Rect> faces;
	cv::Mat img_gray;

	cvtColor(img, img_gray, CV_BGR2GRAY);
	cv::equalizeHist(img_gray, img_gray);
	const int scale = 2;

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
		std::cout << "Haarcascade_frontalface_alt.xml not found." << std::endl;
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