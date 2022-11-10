#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


int main() {
	Mat img1 = imread("./imgs/szita2.png", IMREAD_GRAYSCALE);
	Mat img2 = imread("./imgs/melyseg.png", IMREAD_GRAYSCALE);

	if (img1.empty() || img2.empty()) {
		cout << "Az kep nem olvashat�." << endl;
		exit(-1);
	}

	int szita_y = 200;
	int melyseg_y = 500;

	imshow("szitakoto", img1);
	moveWindow("szitakoto", 100, szita_y);
	imshow("melyseg", img2);
	moveWindow("melyseg", 100, melyseg_y);

	namedWindow("szuro", WINDOW_NORMAL);
	resizeWindow("szuro", 500, 100);


	int radius = 1;
	createTrackbar("sugar", "szuro", &radius, 50);

	int sigma = 10;
	createTrackbar("sigma", "szuro", &sigma, 50); // 1-5 k�z�ttit fog adni, mert majd 10-el osztom

	namedWindow("gauss", WINDOW_NORMAL);
	resizeWindow("gauss", 128, 128);

	int q, ksize = 1;
	Mat b1, mb1, gb1;
	Mat b2, mb2, gb2;

	Mat gk;

	int first = 1;
	while ((q = waitKey(40)) != 'q') {
		ksize = max(2 * radius + 1, 3);
		sigma = max(sigma, 10);

		blur(img1, b1, Size(ksize, ksize));
		blur(img2, b2, Size(ksize, ksize));

		medianBlur(img1, mb1, ksize);
		medianBlur(img2, mb2, ksize);

		GaussianBlur(img1, gb1, Size(ksize, ksize), sigma/10.0);
		GaussianBlur(img2, gb2, Size(ksize, ksize), sigma/10.0);

		gk = cv::getGaussianKernel(ksize, sigma / 10.0);
		gk *= gk.t();
		double a, b;
		minMaxLoc(gk, &a, &b);
		gk = (gk - a) * 1.0 / (b - a);
		imshow("gauss", gk);

		imshow("kornyezeti atlag 1", b1);
		imshow("kornyezeti atlag 2", b2);
		imshow("median szuro 1", mb1);
		imshow("median szuro 2", mb2);
		imshow("gauss szuro 1", gb1);
		imshow("gauss szuro 2", gb2);
		if (first)
		{
			first = 0;
			moveWindow("kornyezeti atlag 1", 400, szita_y);
			moveWindow("kornyezeti atlag 2", 400, melyseg_y);
			moveWindow("median szuro 1", 1200, szita_y);
			moveWindow("median szuro 2", 1200, melyseg_y);
			moveWindow("gauss szuro 1", 800, szita_y);
			moveWindow("gauss szuro 2", 800, melyseg_y);
		}

	}

	waitKey(0);

	return 0;
}