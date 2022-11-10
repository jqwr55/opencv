/**
* OpenCV gyakorlat
* Gradiens sz�m�t�s Sobel oper�torral
*/

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

#include "common.h"

int main() {

	Mat img = imread("./imgs/go2.png", IMREAD_GRAYSCALE);

	assert(!img.empty());

	//Sz�r�s (tomp�tja a zajokat)	
	cv::GaussianBlur(img, img, cv::Size(5, 5), 1.6, 1.6);
	
	//Deriv�lt k�zel�t�se
	Mat gx, gy, g;
	Sobel(img, gx, CV_16S, 1, 0);  // fuggoletes iranyu elek kiemelese
	Sobel(img, gy, CV_16S, 0, 1);  // vizszintes iranyu elek kiemelese 
	g = abs(gx) + abs(gy);  //a deriv�lt
	
	megjelenit("x szerinti derivalt", gx); 
	megjelenit("y szerinti derivalt", gy); 
	megjelenit("derivalt kozelitese", g);  


	//Deriv�lt val�s sz�mokkal
	Mat gxv, gyv, gv;
	Sobel(img, gxv, CV_32F, 1, 0);
	Sobel(img, gyv, CV_32F, 0, 1);
	//a deriv�lt: a mul �s az sqrt val�s m�trixot ig�nyel
	cv::sqrt(gxv.mul(gxv) + gyv.mul(gyv), gv);  

	megjelenit("derivalt", gv);

	waitKey(0);
	
	return 0;
}