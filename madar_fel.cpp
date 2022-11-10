#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "common.h"
#include "histo.h"

using namespace std;
using namespace cv;

int main() {

	Mat img = imread("./imgs/madar.jpg", IMREAD_COLOR);

	if (img.empty()) {
		cout << "Upsz" << endl;
		exit(-1);
	}

    Mat channels[3];
    split(img, channels);

	imshow("eredeti kep", img);
	imshow("B", channels[0]);

    Mat mask;
    threshold(channels[0], mask, 110, 255, THRESH_BINARY);
    imshow("mask", mask);

    Mat finalImg = Mat::zeros( img.cols, img.rows, img.type()); 
    img.copyTo(finalImg, mask);
    imshow("final", finalImg);

    /*
	Histo::showHisto(img, "eredeti hiszto", 1);

	Mat mask;
	inRange(img, 165, 213, mask);

	imshow("sav (maszk)", mask);

	medianBlur(mask, mask, 7);

	img.setTo(0, 255 - mask);

	imshow("modositott kep", img);
    */

	waitKey(0);
	return 0;
}