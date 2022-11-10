#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "histo.h"

using namespace std;
using namespace cv;

/// <summary>
/// Szintorzulashoz vezethet!
/// </summary>
/// <param name="img"></param>
/// <param name="dest"></param>
void equalizeHistColor1(const Mat img, Mat& dest) {
	vector<Mat> chs;
	split(img.clone(), chs);
	equalizeHist(chs[0], chs[0]);
	equalizeHist(chs[1], chs[1]);
	equalizeHist(chs[2], chs[2]);
	merge(chs, dest);
}

void equalizeHistColor2(const Mat img, Mat& dest) {
	Mat lab;
	cvtColor(img, lab, COLOR_BGR2Lab);

	vector<Mat> chs;
	split(lab, chs);

	equalizeHist(chs[0], chs[0]);

	Mat dest_lab;
	merge(chs, dest_lab);

	cvtColor(dest_lab, dest, COLOR_Lab2BGR);
}


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