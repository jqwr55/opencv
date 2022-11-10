#include "common.h"
using namespace std;

string category(double diff) {
	if (diff < 0.5)
		return "Szemre nem erzekelheto.";

	if (diff < 1.5)
		return "Alig erzekelheto.";

	if (diff < 3.0)
		return "Erzekelheto.";

	if (diff < 6.0)
		return "Jol lathato.";

	return "Nagy szininger kulonbseg.";
}


void convert_labf(const Mat rgb, Mat& dest) {
	Mat rgbf;
	rgb.convertTo(rgbf, CV_32FC3, 1 / 255.0);

	// Az elozo atalakitas miatt a dest kep is CV_32FC3-as lesz, 
	//ami a kesobbi szamitasok miatt erdekes
	cvtColor(rgbf, dest, COLOR_BGR2Lab);
}

int main() {

	Mat img = imread("./imgs/orange1.jpg");

	Mat img_lab;
	convert_labf(img, img_lab);
	imwrite("./imgs/lab.png", img_lab);

	// a bal felső és a jobb alsó sarokban álló pont Lab színtérbeli teljes színkülönbsége
	Vec3f p1 = img_lab.at<Vec3f>(0, 0);
	Vec3f p2 = img_lab.at<Vec3f>(img.rows-1, img.cols-1);
	double diff = cv::norm(p1 - p2);
	std::cout << category(diff) << endl;

	// a bal felső sarokban lévő és a tőle jobbra lévő szomszéd teljes színkülönbsége
	p1 = img_lab.at<Vec3f>(0, 0);
	p2 = img_lab.at<Vec3f>(0, 1);
	diff = cv::norm(p1 - p2);
	std::cout << category(diff) << endl;

	waitKey(0);
	return 0;
}