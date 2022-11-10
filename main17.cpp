#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "histo.h"

using namespace std;
using namespace cv;

int main() {

	Mat img = imread("./imgs/gray_buttons.jpg", IMREAD_GRAYSCALE);

	if (img.empty()) {
		cout << "Upsz" << endl;
		exit(-1);
	}
	imshow("eredeti kep", img);
	Histo::showHisto(img, "eredeti hiszto", 1);

	Mat mask;
	inRange(img, 165, 213, mask);

	imshow("sav (maszk)", mask);

	medianBlur(mask, mask, 7);

	img.setTo(0, 255 - mask);

	imshow("modositott kep", img);

	waitKey(0);
	return 0;
}