/*
OpenCV segadanyagok:
Egyszeru pelda adaptiv kuszobolesre.

Adaptiv kuszobolest csak akkor erdemes hasznalni, amikor egy globalis kuszobertek hasznalata nem vezetne eredmenyre,
ugyanis eroforrasigenyesebb, mint a 'sima' kuszoboles.
Pl. egyenetlen megvilagitas eseteben.  
*/
#include <iostream>
#include <opencv2/core.hpp>    // osztalyok, strukturak, alapveto funkciok
#include <opencv2/highgui.hpp> // felulet
#include <opencv2/imgproc.hpp> // kepfeld. fuggvenyek

using namespace std;
using namespace cv;

int main() {
	Mat img = imread("./imgs/zh.jpg", IMREAD_GRAYSCALE);
	assert(!img.empty());

	resize(img, img, Size(img.cols / 1.5, img.rows / 1.5) );

	imshow("img", img);
	int value = 20;
	int c = 25;
	createTrackbar("size: ", "img", &value, 100);
	createTrackbar("c: ", "img", &c, 50);
	
	Mat dest;
	int pre_c = 0, pre_v = 0;
	while (waitKey(5) != 'q') {
		// valtoztak-e az input ertekek
		if (pre_c != c || pre_v != value) {
			// a pont koruli ablak ala eso pontok atlagat (Gauss eseten sulyozott atlagat) szamita ki
			// azon pontok lesznek eloterpontok az eredmenykepen, melyek az atlag-C erteknel nagyobb intenzitassal birnak
			adaptiveThreshold(img, dest, 255, AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, (value * 2) + 3, c);
			imshow("img", dest);
		}
	}
	return 0;
}