#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {

	Mat img = imread("./imgs/sejtek.png", IMREAD_GRAYSCALE);

	assert(!img.empty());

	Mat dest;
	string ablaknev;

	int ablak_meret = 300;
	int x = 0, y = 0;

	for (int th = 20; th < 255; th += 20) {
		//ablak n�v a k�sz�b megjel�l�s�vel
		ablaknev = "th: " + std::to_string(th);
		//ablak l�trehoz�sa
		namedWindow(ablaknev, WINDOW_NORMAL);
		//k�sz�b�l�s fix hat�rral
		cv::threshold(img, dest, th, 255, THRESH_BINARY_INV);

		imshow(ablaknev, dest);		
		resizeWindow(ablaknev, Size(ablak_meret, ablak_meret));
		moveWindow(ablaknev, x, y);

		if (x > 1000) {
			x = 0;
			y += ablak_meret+34; //a fejl�c miatt egy kis plusz
		}
		else {
			x += ablak_meret;
		}
	}

	waitKey(0);
	destroyAllWindows();


	return 0;
}