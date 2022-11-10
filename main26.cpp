/**
* OpenCV gyakorlat
* Hough kordetektalo 
	- a Canny detektalora epit, ezert erdemes lehet elobb a canny eldetektalot magaban futtatni a kepre, hogy a kuszoberteket beallitsd
	- ha ismert a kor merete, feltetlenul hasznald ki, mert jelentosen gyorsit
	- ha nem ad vissza kort, lazits a parametereken, elofordulhat, hogy mas tulajdonsagok alapjan, utolag kell majd szurni a megfelelo koroket
*/

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "common.h"

using namespace std;
using namespace cv;

/// <summary>
/// Sobelre epit
/// </summary>
/// <param name="gray"></param>
/// <param name="circles"></param>

int main() {

	Mat colorImg = imread("./imgs/go2.png");

	assert(!colorImg.empty());

	Mat gray;
	cvtColor(colorImg, gray, COLOR_BGR2GRAY);

	vector<Vec3f> circles; // Vec3f: [0]: kozeppont x,  [1] kozeppont y, [2] subar
	kordetektalo1(gray, circles);
	//kordetektalo2(gray, circles);


	//Korok kirajzolasa
	Scalar feketebabu(0, 0, 255);
	Scalar feherbabu(0, 255, 0);
	Scalar hamis(128, 128, 128);
	int x, y, r;

	for (auto c : circles) {
		x = c[0];
		y = c[1];
		r = c[2];
		if (gray.at<uchar>(y, x) < 50)
			 circle(colorImg, Point(x, y), r, feketebabu, 2);
		else if (gray.at<uchar>(y, x) > 150)
			circle(colorImg, Point(x, y), r, feherbabu, 2);
		else //nem is feher, nem is fekete, utolag is kidobhato a korok kozol, ami nem kell
			 //szukseg is lehet ra, ha nem talalunk olyan parametereket, ami tokeletes eredmenyt ad
			circle(colorImg, Point(x, y), r, hamis, 2);
	}


	imshow("babuk", colorImg);
	waitKey(0);

	return 0;
}