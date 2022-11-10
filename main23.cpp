/*
OpenCV segadanyagok:
Gradiens szamitas, gradiens erossegen alapulo eldetektals.  
*/

#include <iostream>
#include <opencv2/core.hpp>    // osztalyok, strukturak, alapveto funkciok
#include <opencv2/highgui.hpp> // felulet
#include <opencv2/imgproc.hpp> // kepfeld. fuggvenyek
#include "common.h"

using namespace std;
using namespace cv;

int main() {
	//bekerunk egy kepet a sorozatban szereplo kepek meretenek megismeresehez
	Mat img = imread("./imgs/usa_flag.jpg", IMREAD_GRAYSCALE);

	Mat dest_roberts;
	roberts(img, dest_roberts);

	Mat dest_sobel_approx;
	sobel(img, dest_sobel_approx);

	Mat dest_sobel;
	sobel(img, dest_sobel);

	Mat dest_laplace;
	Laplacian(img, dest_laplace, CV_8UC1, 3);

	imshow("eredeti", img);
	imshow("roberts", dest_roberts);
	imshow("sobel_approx", dest_sobel_approx);
	imshow("sobel", dest_sobel);
	imshow("laplacian", dest_laplace);

	waitKey(0);

	// egesz jonak tunik, de nezzuk mi a helyzet valojaban:
	// a zajos kep azt jelenti, hogy meg 50-es erossegu hamis elek is vannak a kepen
	// termeszetesen emelheto a hatar tovabb, es egyre kevesebb zaj marad
	// de vigyazz, mert ha tul sokaig emeled, akkor a valodi elek is odavesznek
	// Jo ha tudod: sok kep eseteben nem fogsz megfelelo hatart talalni! 
	        //(kapcsolodo temak: zajszures, konturkovetes)
	int kuszob = 50;

	// a zaj es a valodi elek elvalasztasa kuszobolessel 
	// a hatar mindig problemafuggo, a megtalalasa sokszor nem trivialis
	// konnyen megszakadhatnak az elek (megoldas: konturkovetes: ld. Canny)
	threshold(dest_roberts, dest_roberts, kuszob, 255, THRESH_BINARY);
	imshow("elek: roberts + threshold", dest_roberts);

	threshold(dest_sobel_approx, dest_sobel_approx, kuszob, 255, THRESH_BINARY);
	imshow("elek: sobel_approx + threshold", dest_sobel_approx);

	threshold(dest_sobel, dest_sobel, kuszob, 255, THRESH_BINARY);
	imshow("elek: sobel + threshold", dest_sobel);

	threshold(dest_laplace, dest_laplace, kuszob, 255, THRESH_BINARY);
	imshow("elek: roberts + threshold", dest_laplace);

	waitKey(0);

	return 0;
}