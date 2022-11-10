///
/// Kepfeld segedlet - kontur tarolasi modok
/// 
 
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


int main() {
	Mat img = imread("./imgs/objektumok.png", IMREAD_COLOR);
	Mat bin_img;
	cvtColor(img, bin_img, COLOR_BGR2GRAY);

	// ---------------  kulso kontur, minden pont tarolasa --------------
	vector<vector<Point>> contours;
	findContours(bin_img.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	Mat dest1 = img.clone();
	drawContours(dest1, contours, -1, Scalar(0, 0, 255), 2);
	for (auto c : contours) {
		putText(dest1, to_string(c.size()), c[0], FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 255, 0));
	}

	// ------  kulso kontur, tomorites (lanckod egyszeru tomoritese) -----
	vector<vector<Point>> contours2;
	findContours(bin_img.clone(), contours2, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	Mat dest2 = img.clone();
	drawContours(dest2, contours, -1, Scalar(0, 0, 255), 2);
	for (auto c : contours2) {
		putText(dest2, to_string(c.size()), c[0], FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 255, 0));
	}

	// ---  kulso kontur, a kontur valodi apporximacioja ----
	Mat dest3 = img.clone();
	drawContours(dest3, contours, -1, Scalar(0, 0, 255), 2);
	for (auto c : contours) {
		vector<Point> approxed_contours;
		approxPolyDP(c, approxed_contours, 5, true);
		polylines(dest3, approxed_contours, true, Scalar(255, 0, 0), 2);
		putText(dest3, to_string(approxed_contours.size()), c[0], FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 255, 0));
	}


	imshow("approx_none", dest1);
	imshow("approx_simple", dest2);
	imshow("approx_polygon", dest3);


	waitKey(0);
}