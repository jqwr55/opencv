/*
OpenCV segadanyagok:
Pelda morfologiai szurore
*/

#include <iostream>
#include <opencv2/core.hpp>    // osztalyok, strukturak, alapveto funkciok
#include <opencv2/highgui.hpp> // felulet
#include <opencv2/imgproc.hpp> // kepfeld. fuggvenyek
#include "common.h" // kepfeld. fuggvenyek

using namespace std;
using namespace cv;

int main() {

	Mat img = imread("./imgs/sajt.png", IMREAD_COLOR);

    Mat gray = img.clone();
    cvtColor(gray, gray, COLOR_BGR2GRAY);

    imshow("og", img);
    int kernel = 2;
    int itErode = 1;
    int itDilate = 1;
    int lth = 0;
    int hth = 0;
    int ratio = 50;
    createTrackbar("se", "og", &kernel, 10);
    createTrackbar("it erode", "og", &itErode, 20);
    createTrackbar("it dilate", "og", &itDilate, 20);
    createTrackbar("lth", "og", &lth, 25);
    createTrackbar("hth", "og", &hth, 255);
    createTrackbar("ratio", "og", &ratio, 100);

    int key = 0;
    while( (key = waitKey(40)) != 'q' ) {

		auto th = calc_th_value(img, (float)ratio / 100.f);
        Mat binary_img = Mat::zeros(Size(img.rows, img.cols), CV_16UC1);
		threshold(gray, binary_img, th, 255, THRESH_BINARY);

        kernel = max(kernel, 2);
        Mat se = getStructuringElement(MORPH_CROSS, Size(kernel, kernel));
        
        // zaj eltavolitasa: az erozio 5x5-os szurovel eltavolitaja az ennel kisebb zajokat
        // a dilatacioval a korvonal helyreall valamennyire
        // hasonlo a helyzet, mint a medianszuro eseteben: novelhetjuk a kernel meretet, ha tobb zajt akaruk eltavolitani
        // de a kutya orra is deformalodik majd rendesen

        Mat tmp, dest;
#if 0
        morphologyEx(binary_img, dest, MORPH_OPEN, se, Point(-1,-1), itErode);

#else

        erode(binary_img, binary_img, se, Point(-1,-1), itErode);
        dilate(binary_img, binary_img, se, Point(-1,-1), itDilate);
#endif

        binary_img = 255 - binary_img;
        Mat composite = img.clone();
        bitwise_and(gray, binary_img, composite);

        cvtColor(composite, composite, COLOR_GRAY2BGR);
    
        imshow("final", composite);
        imshow("binary_img", binary_img);
        moveWindow("binary_img", 500, 0);
        // moveWindow("final", 100, 0);
    }


	waitKey(0);
}