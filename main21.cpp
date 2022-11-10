/**
* OpenCV gyakorlat
* K�sz�b�l�s: iterat�v m�don
* Annak a k�sz�b�rt�knek a meghat�roz�sa, ahol az el�t�r az eg�sz k�pnek valah�ny % lesz.
*/

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "common.h"

using namespace std;
using namespace cv;

int main() {

	//Input: sz�rkesk�l�s k�p (vagy eleve sz�rkesk�l�ban olvassuk be vagy a cvtColor-ral �talak�tunk egy szineset)
	Mat img = imread("./imgs/scanned3.png", IMREAD_GRAYSCALE);

	assert(!img.empty());

	cv::Mat histo;
	createHisto(img, histo);
	
	//ha nem tudod mit ad vissza, k�rd le a m�retet
	//cout << histo.size() << endl;
	
	//ha nem tudod, hogy milyen k�p ker�l vissza, k�rd le a t�pust (interface.h-ban vannak a defek)
	//cout << histo.type() << endl;  

	float osszes = img.cols * img.rows; //�sszes k�ppont sz�ma
	float hatter = 0.10f; 

	Mat dest;
	float s = 0;
	float th = 0;
	for (int i = 0; i < histo.rows; ++i) {
		s += histo.at<float>(i);
		if ( s / osszes > hatter ) {
			th = i;
			break;
		}
	}

	auto th2 = calc_th_value(img, 0.1);
	threshold(img, dest, th, 255, THRESH_BINARY_INV);
	Mat dst2;
	threshold(img, dst2, th2, 255, THRESH_BINARY_INV);

	imshow("arany", dest);
	imshow("dst", dst2);
	waitKey();

	return 0;
}