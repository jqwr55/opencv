#include <iostream>
#include <opencv2/core.hpp>    // osztalyok, strukturak, alapveto funkciok
#include <opencv2/highgui.hpp> // felulet
#include <opencv2/imgproc.hpp> // kepfeld. fuggvenyek

using namespace std;
using namespace cv;

int main() {
	//bekerunk egy kepet a sorozatban szereplo kepek meretenek megismeresehez
	Mat img = imread("./imgs/SnapShot-20180731_173715.jpg");

	//a kap alapjan letrehozhatjuk a matrixot, mely a kepek pontonkenti osszeget fogja tarolni
	Mat acc = Mat::zeros(img.size(), CV_64FC3);

	//egy szamlalo, az atlagszamitashoz
	int n = 0;

	//a sorozat kepei
	for (int i = 715; i <= 918; ++i) {
		img = imread("./imgs/SnapShot-20180731_173" + to_string(i)+".jpg");
		if (img.empty())
			continue;
		
		accumulate(img, acc);
		++n;

		// "video lejatszo" resz
		imshow("img", img);
		waitKey(5);
	}

	//atlagolunk es visszaterunk a szokasos 8 bit/csatorna tipushoz
	Mat dest;
	acc.convertTo(dest, CV_8UC3, 1.0/n);

	imshow("szurt", dest);
	waitKey(0);
}