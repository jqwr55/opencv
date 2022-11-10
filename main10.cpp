#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "histo.h"

using namespace std;
using namespace cv;

int main() {

	Mat img = imread("./imgs/orange1.jpg", IMREAD_COLOR);

	if (img.empty()) {
		cout << "Upsz" << endl;
		exit(-1);
	}

	Histo::showHisto(img, "eredeti hiszto", 1);
	

	// ---------- 1. modszer - szintorzitas eselye nagy -------------
	Mat dest;
	equalizeHistColor1(img, dest);
	
	Histo::showHisto(dest, "kiegy. hiszto 1", 1);

	imshow("eredeti kep", img);
	imshow("kiegy. kep 1", dest);

	waitKey(0);
	destroyAllWindows();

	// ---------- 2. modszer -------------

	Histo::showHisto(img, "eredeti hiszto", 1);

	Mat dest2;
	equalizeHistColor2(img, dest2);

	Histo::showHisto(dest2, "kiegy. hiszto 2", 1);

	imshow("eredeti kep", img);
	imshow("kiegy. kep 2", dest2);

	waitKey(0);
	destroyAllWindows();
	return 0;
}