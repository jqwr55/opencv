#include "common.h"

using namespace std;

void kuramaMasol(const Mat fg, Mat& bg) {
	Vec3b c;
	resize(bg, bg, fg.size());
	for (int i = 0; i < fg.rows; ++i) {
		for (int j = 0; j < fg.cols; ++j) {
			c = fg.at<Vec3b>(i, j);
			if (c[0] < 245 || c[1] < 245 || c[2] < 245) {
				bg.at<Vec3b>(i, j) = c; 
			}
		}
	}
}


/// <summary>
/// Mini kurama masolo
/// </summary>
/// <param name="fg">eloterkep</param>
/// <param name="bg">hatterkep (InputOutput)</param>
void kuramaMasol2(const Mat fg, Mat& bg) {

	Vec3b c;
	Mat tmp;

	//Kuramat zsugoritjuk 
	//(megj.: ha eredetileg 4-gyel oszthato a kep akkor fg.Size()/4-es is ok, de ellenorzes nelkul
	// kockazatos
	resize(fg, tmp, Size(fg.cols/4, fg.rows/4));

	// az offsx biztositja majd, hogy a kep kozepere keruljon kurama vizszintes iranyban
	int offsx = bg.cols/2 - tmp.cols/2;
	// a fuggolegest ugy allitjuk, hogy "rauljon" a foldre
	int offsy = bg.rows/2 + 50;

	for (int i = 0; i < tmp.rows; ++i) {
		for (int j = 0; j < tmp.cols; ++j) {
			c = tmp.at<Vec3b>(i, j);
			if (c[0] < 245 || c[1] < 245 || c[2] < 245) {
				if (i+offsy < bg.rows and j+offsy < bg.cols) 
				   bg.at<Vec3b>(i + offsy, j + offsx) = c;
			}
		}
	}
	// egy kis elmosas, errol majd kesobb, egyelore eleg annyi, 
	// hogy beleolvasztja egy picit Kuramat a kornyezetebe
	cv::GaussianBlur(bg, bg, Size(5, 5), 1.2);
}

int main() {

	Mat fg = imread("./imgs/kurama.jpg");
	Mat bg = imread("./imgs/background.jpg");
	//kuramaMasol(fg, bg);
	kuramaMasol2(fg, bg);

	imshow("abrakadabra", bg);
	waitKey(0);

	return 0;
}