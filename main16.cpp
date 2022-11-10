#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//#include "histo.h"

using namespace std;
using namespace cv;


int main() {

	Mat img = imread("./imgs/dog.jpg", IMREAD_GRAYSCALE);

	if (img.empty()) {
		cout << "Upsz" << endl;
		exit(-1);
	}

	//Histo::showHisto(img, "eredeti hiszto", 1);
	
	// ------------------- feladat 1 ---------------
	Mat dest;
	threshold(img, dest, 100, 255, THRESH_BINARY);

	// nagyobb szurovel a kutya pofaja mar borzasztoan erodalodik
	medianBlur(dest, dest, 5);

	imshow("kuszobolt", dest);

	// ------------------- feladat 2 ---------------
	Mat dest2;
	threshold(img, dest2, 100, 255, THRESH_TOZERO);

	imshow("eredeti kep", img);
	imshow("vagott", dest2);

	waitKey(0);
	return 0;
}