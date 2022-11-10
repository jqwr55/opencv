#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "histo.h"

using namespace std;
using namespace cv;



int main() {

	Mat img = imread("./imgs/dark_img.jpg", IMREAD_GRAYSCALE);

	if (img.empty()) {
		cout << "Upsz" << endl;
		exit(-1);
	}

	Histo::showHisto(img, "eredeti hiszto", 1);
	
	double ah, fh;
	minMaxLoc(img, &ah, &fh);
	
	//fh = 60;

	Mat dest = (img - ah) * (255.0 / (fh - ah));
	Histo::showHisto(dest, "nyujtott hiszto");

	imshow("eredeti kep", img);
	imshow("kontraszt nyujtott kep", dest);
	waitKey(0);
	return 0;
}