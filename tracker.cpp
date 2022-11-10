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

	Mat img = imread("./imgs/sajt.png", IMREAD_GRAYSCALE);

    imshow("og", img);
    int kernel = 2;
    int itErode = 1;
    int itDilate = 1;
    int lth = 0;
    int hth = 100;
    createTrackbar("se", "og", &kernel, 10);
    createTrackbar("it erode", "og", &itErode, 10);
    createTrackbar("it dilate", "og", &itDilate, 10);
    createTrackbar("lth", "og", &lth, 25);
    createTrackbar("hth", "og", &hth, 255);

    int key = 0;
    while( (key = waitKey(40)) != 'q' ) {

        Mat binary_img;
        threshold(img, binary_img, lth, hth, THRESH_BINARY);

        Mat bin2 = binary_img.clone();

        kernel = max(kernel, 2);
        Mat se = getStructuringElement(MORPH_ELLIPSE, Size(kernel, kernel));
        
        // zaj eltavolitasa: az erozio 5x5-os szurovel eltavolitaja az ennel kisebb zajokat
        // a dilatacioval a korvonal helyreall valamennyire
        // hasonlo a helyzet, mint a medianszuro eseteben: novelhetjuk a kernel meretet, ha tobb zajt akaruk eltavolitani
        // de a kutya orra is deformalodik majd rendesen

        Mat tmp, dest, dest2;
#if 1
        morphologyEx(binary_img, dest, MORPH_OPEN, se, Point(-1,-1), itErode);

#else

        erode(binary_img, binary_img, se, Point(-1,-1), itErode);
        dilate(binary_img, binary_img, se, Point(-1,-1), itDilate);
#endif

        imshow("binary_img", binary_img);

        Mat composite;
        img.copyTo(composite, binary_img);
        imshow("final", composite);
        imshow("bin2", bin2);
    
        moveWindow("binary_img", 500, 0);
        moveWindow("final", 100, 0);

        std:cout << itErode << " " << itDilate << endl;
    }
    /*
    */
	waitKey(0);
}