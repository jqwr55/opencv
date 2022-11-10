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
	
	Mat dest;
	equalizeHist(img, dest);
	
	Histo::showHisto(dest, "kiegyenlitett hiszto");

	imshow("eredeti kep", img);
	imshow("kiegyenlitett kep", dest);
	waitKey(0);
	return 0;
}