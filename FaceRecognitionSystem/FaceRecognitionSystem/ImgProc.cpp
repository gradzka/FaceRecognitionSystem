#include "ImgProc.h"

#define crop_width 200
#define crop_height 200


ImgProc::ImgProc()
{
	//face_cascade_name = "haarcascade_frontalface_alt.xml";
	face_cascade_name = "lbpcascade_frontalface.xml";
	window_name = "Hello Face!";
	model = cv::createEigenFaceRecognizer();
	//model = cv::createLBPHFaceRecognizer();
	//model = cv::createFisherFaceRecognizer();
	isModelTrained = false;
	//model->set("threshold", 150.0);
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
	std::string folderName = "";
	for (auto & p : std::experimental::filesystem::recursive_directory_iterator(path))
	{
		if (!std::experimental::filesystem::is_directory(p.path()))
		{
			file << p << ";" << numberPerson << std::endl;
			
		}
		else
		{
			numberPerson++;
			peopleBase[numberPerson] = p.path().string().substr(12, p.path().string().length()-12);
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
	vcap.release();
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
			createCSV();
			read_csv("corp.csv", images, labels);
			isModelTrained = true;
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
	isModelTrained = true;
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
	if (isModelTrained==false)
	{
		std::cout << "Error! Model is not trained!" << std::endl;
		return;
	}
	std::cout << std::endl << "wait..." << std::endl;
	cv::VideoCapture vcap;
	cv::Mat image;
	const std::string videoStreamAdress = "rtsp://" + userPwd + "@" + addressIP + ":80/live/ch0";

	if (face_cascade.load(face_cascade_name))
	{
		std::vector<cv::Rect> faces;
		cv::Mat img_gray;
		cv::Mat img_cut;
		cv::Mat img_resized;
		cv::Mat resized_gray;

		int plabel = -1;
		double predicted_confidence = 0.0;
		int unrecogizedPeople = 0;
		int positionInFile = 1;
		
		char answer;
		do
		{
			std::cout << "1 - run FaceRecognizer in endless mode" << std::endl;
			std::cout << "2 - run FaceRecognizer in time mode" << std::endl;
			std::cin >> answer;
		} while (answer != '1' && answer != '2');

		int secs = 0;
		if (answer == '2')
		{
			std::string runTime = "";
			int h, m, s, z = 0;
			do
			{
				std::cout << "Type duration time of FaceRecognizer in format HH:mm:ss" << std::endl;
				std::cin >> runTime;
			} while (sscanf(runTime.c_str(), "%d:%d:%d", &h, &m, &s) != 3);
			secs = h * 3600 + m * 60 + s;
		}

		int frameID = -1;

		//Create file with presences info
		std::fstream pfile;
		//file name
		std::time_t time;
		char date[20];
		std::experimental::filesystem::create_directory("presence/");
		std::string fileName = "presence/";
		struct tm timeinfo;
		time = std::time(NULL);
		localtime_s(&timeinfo, &time);
		if (std::strftime(date, sizeof(date), "%F %H-%M-%S", &timeinfo))
		{
			fileName += std::string(date)+".txt";
			pfile.open(fileName, std::fstream::out);
			if (pfile.is_open())
			{
				if (!vcap.open(videoStreamAdress))
				{
					std::cout << "Error opening video stream or file." << std::endl;
					return;
				}
				std::cout << "People recognition module has started..." << std::endl;

				if (answer == '1') 
				{
					while (true)
					{
						if (vcap.read(image))
						{
							frameID = vcap.get(CV_CAP_PROP_POS_FRAMES);//current frame number
							if (frameID % 25 == 0 && frameID != 0)
							{
								face_cascade.detectMultiScale(image, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
								//std::cout << faces.size() << std::endl;
								for (unsigned i = 0; i < faces.size(); i++)
								{
									cvtColor(image, img_gray, CV_BGR2GRAY);
									img_cut = img_gray(faces[i]);
									resize(img_cut, img_resized, cv::Size(crop_width, crop_height), 1.0, 1.0, cv::INTER_CUBIC);
									model->predict(img_resized, plabel, predicted_confidence);

									if (plabel < 0)
									{
										std::cout << "Unrecognized person!" << std::endl;
										unrecogizedPeople++;
									}
									else if (predictedPeople.count(plabel) == 0)
									{
										predictedPeople[plabel] = peopleBase[plabel];
										pfile << positionInFile << "." << " " << predictedPeople[plabel] << std::endl;
										positionInFile++;

									}

									if (plabel >= 0)
									{
										std::cout << "Predicted person: " << predictedPeople[plabel] <<
											" with confidence: " << predicted_confidence <<
											"." << std::endl;
										imshow("Sample", img_resized);
										imshow("Predicted", images[5 * plabel]);
									}
								}
							}
							cv::waitKey(1);
						}
					}
				}
				else
				{
					std::chrono::time_point<std::chrono::steady_clock> end;
					auto start = std::chrono::steady_clock::now();
					while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count()<secs)
					{
						end = std::chrono::steady_clock::now();
						if (vcap.read(image))
						{
							frameID = vcap.get(CV_CAP_PROP_POS_FRAMES);//current frame number
							if (frameID % 25 == 0 && frameID != 0)
							{
								face_cascade.detectMultiScale(image, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
								//std::cout << faces.size() << std::endl;
								for (unsigned i = 0; i < faces.size(); i++)
								{
									cvtColor(image, img_gray, CV_BGR2GRAY);
									img_cut = img_gray(faces[i]);
									resize(img_cut, img_resized, cv::Size(crop_width, crop_height), 1.0, 1.0, cv::INTER_CUBIC);
									model->predict(img_resized, plabel, predicted_confidence);

									if (plabel < 0)
									{
										std::cout << "Unrecognized person!" << std::endl;
										unrecogizedPeople++;
									}
									else if (predictedPeople.count(plabel) == 0)
									{
										predictedPeople[plabel] = peopleBase[plabel];
										pfile << positionInFile << "." << " " << predictedPeople[plabel] << std::endl;
										positionInFile++;

									}

									if (plabel >= 0)
									{
										std::cout << "Predicted person: " << predictedPeople[plabel] <<
											" with confidence: " << predicted_confidence <<
											"." << std::endl;
										imshow("Sample", img_resized);
										imshow("Predicted", images[5 * plabel]);
									}
								}
							}
							cv::waitKey(1);
						}
					}
				}
				pfile << unrecogizedPeople << " - " << "unrecognized people!" << std::endl;
				pfile.close();
			}
			else
			{
				std::cout << face_cascade_name + " not found." << std::endl;
			}
		}
		else
		{
			std::cout << fileName << " is corrupted!" << std::endl;
			pfile.close();
		}

	}
	else
	{
		std::cout << "Creating presence file has been corrupted!" << ::std::endl;
	}
	std::cout << "FaceRecognizer was stopped successfully." << std::endl;
}

cv::Mat ImgProc::cropFace(cv::Mat img)
{
	if (face_cascade.load(face_cascade_name))
	{
		std::vector<cv::Rect> faces;
		cv::Mat img_gray;
		cv::Mat img_cut;
		cv::Mat img_resized;
		cv::Mat resized_gray;
		cvtColor(img, img_gray, CV_BGR2GRAY);

		face_cascade.detectMultiScale(img, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
		if (faces.size() != 1)
		{
			return img;
		}
		img_cut = img(faces[0]);
		resize(img_cut, img_resized, cv::Size(crop_width, crop_height), 1.0, 1.0, cv::INTER_CUBIC);
		cvtColor(img_resized, resized_gray, CV_BGR2GRAY);
		return resized_gray;
	}
	else
	{
		std::cout << face_cascade_name + " not found." << std::endl;
		return img;
	}
}