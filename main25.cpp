/**
* OpenCV gyakorlat
* �ldetekt�l�s Sobel �s Canny oper�tor seg�ts�g�vel
*/

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "common.h"

using namespace std;
using namespace cv;

///
/// �ldetekt�l�s Sobel oper�torral
///

int main() {

	Mat img = imread("./imgs/go2.png", IMREAD_GRAYSCALE);

	assert(!img.empty());

	//Sz�r�s (tomp�tja a zajokat)	
	cv::GaussianBlur(img, img, cv::Size(5, 5), 1.6, 1.6);

	Mat sobel, canny;
	SobelElek(img, sobel);
	CannyElek(img, canny, 25);

	imshow("Sobel elek", sobel);
	imshow("Canny elek", canny);
	waitKey(0);

	return 0;
}