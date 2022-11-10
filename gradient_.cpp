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

void compute_g1(const Mat src, Mat& dst) {

    for(int x = 0; x < src.cols - 1; x++) {

        for(int y = 0; y < src.rows - 1 ; y++) {

            dst.at<unsigned short>(y, x) = src.at<unsigned char>(y, x) - src.at<unsigned char>(y+1, x+1);
        }
    }
}

int main() {

	Mat img = imread("./imgs/go2.png", IMREAD_GRAYSCALE);
	assert(!img.empty());

    Mat g1( Size(img.cols, img.rows), CV_16S);
    Mat g2( Size(img.cols, img.rows), CV_16S);

    compute_g1(img, g1);

    imshow("img", img);
    imshow("g1", g1);
    imshow("g2", g2);

	waitKey(0);

	return 0;
}