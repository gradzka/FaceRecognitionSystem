#include "ImgProc.h"



ImgProc::ImgProc()
{
	face_cascade_name = "haarcascade_frontalface_alt.xml";
	window_name = "Hello Face!";
}


ImgProc::~ImgProc()
{ }

int ImgProc::detectFace(cv::Mat img)
{
	std::vector<cv::Rect> faces;
	cv::Mat img_gray;

	cvtColor(img, img_gray, CV_BGR2GRAY);

	face_cascade.detectMultiScale(img_gray, faces, 1.06, 3, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
	int percentheigth = 0;
	int percentwidth = 0;
	for (unsigned i = 0; i < faces.size(); i++)
	{
		percentheigth = 0.4*faces[i].height;
		faces[i].height += 1.65*percentheigth;
		faces[i].y -= 1.25*percentheigth;

		percentwidth = 0.4*faces[i].width;
		faces[i].width += 0.40*percentwidth;
		faces[i].x -= 0.175*percentwidth;

		cv::Rect rect_face(faces[i]);
		rectangle(img, rect_face, cv::Scalar(120, 5, 86), 2, 2, 0);
	}
	imshow(window_name, img);
	return faces.size();
}

void ImgProc::detect()
{
	const std::string img_name = "twarz.jpg";

	cv::Mat img;
	img = cv::imread(img_name);
	if (!img.data)
	{
		std::cout << "Nie znaleziono pliku " << img_name << ".";
		return;
	}
	if (!face_cascade.load(face_cascade_name))
	{
		std::cout << "Nie znaleziono pliku " << face_cascade_name << ".";
		return;
	}
	detectFace(img);
	cv::waitKey(0);
}