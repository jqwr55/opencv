///
/// OpenCV segadanyag
/// findContours 
///     �sszef�gg� komponensek keresese
/// 

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


int main() {
	string img_path = "./imgs/gumialatet3.jpg";

	Mat img = imread(img_path);
	if (img.empty()) {
		cout << "a kep nem talalhato" << endl;
		exit(-1);
	}

	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	Mat binary;
	threshold(gray, binary, 200, 255, THRESH_BINARY_INV);

	imshow("img", binary);
	imshow("binary", binary);
	
	// kulso konturokra vadaszunk, de ezuttal a lyukakon beluli objektumok kulso konturjait is 
	// meg akarjuk szerezni => RETR_CCOMP
	vector<vector<Point>> contours;
	vector<Vec4i> hier;
	findContours(binary, contours, hier, RETR_CCOMP, CHAIN_APPROX_NONE);

	for (size_t i = 0; i < contours.size(); ++i) {
		// viszont csak az objektum erdekel, az ureg nem, ezert csak a felso szintet jelenitjuk meg
		if (hier[i][3] == -1) {
			drawContours(img, contours, i, Scalar(rand() & 255, rand() & 255, rand() & 255), 2);
		}
	}
	imshow("res", img);
	waitKey();
}