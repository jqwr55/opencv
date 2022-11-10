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

	Mat img = imread("./imgs/galaxy.jpg", IMREAD_GRAYSCALE);

	// csak egy picit atmeretezem, 
	// ha a kisebb kepet toltottek le, amit az elozo gyakorlatoknak hasznltunk, akkor tegyek megjegyzesbe
	resize(img, img, Size(img.cols / 2, img.rows / 2));

    imshow("og", img);
    int kernel = 2;
    int itErode = 1;
    int itDilate = 1;
    createTrackbar("se", "og", &kernel, 10);
    createTrackbar("it erode", "og", &itErode, 10);
    createTrackbar("it dilate", "og", &itDilate, 10);

    int key = 0;
    while( (key = waitKey(40)) != 'q' ) {

        Mat binary_img;
        threshold(img, binary_img, 100, 255, THRESH_BINARY);

        kernel = max(kernel, 2);
        Mat se = getStructuringElement(MORPH_ELLIPSE, Size(kernel, kernel));
        
        // zaj eltavolitasa: az erozio 5x5-os szurovel eltavolitaja az ennel kisebb zajokat
        // a dilatacioval a korvonal helyreall valamennyire
        // hasonlo a helyzet, mint a medianszuro eseteben: novelhetjuk a kernel meretet, ha tobb zajt akaruk eltavolitani
        // de a kutya orra is deformalodik majd rendesen
        Mat tmp, dest, dest2;
        // morphologyEx(binary_img, dest, MORPH_OPEN, se);

        erode(binary_img, binary_img, se, Point(-1,-1), itErode);
        dilate(binary_img, binary_img, se, Point(-1,-1), itDilate);

        imshow("binary_img", binary_img);


        Mat composite;
        img.copyTo(composite, binary_img);
        imshow("final", composite);
    
        moveWindow("binary_img", 500, 0);
        moveWindow("final", 100, 0);
    }
	waitKey(0);
}