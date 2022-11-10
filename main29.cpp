#include <iostream>
#include <string>
#include <algorithm>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {

	VideoCapture cap;
	cap.open("./imgs/sas.avi");

	if (!cap.isOpened()) {
		cout << "error" << endl;
		return -1;
	}

	int fn = 0;
	Rect r;
	vector<vector<Point>> cont;
	Mat img, gray, mask, dest;
	while (1) {
		cap >> img;
		if (img.empty())
			break;

		//sz�rkesk�l�ss� alak�t�s
		cvtColor(img, gray, COLOR_BGR2GRAY);

		//az �g maszkja
		inRange(gray, 100, 155, mask);

		//a sas maszkja
		mask = 255 - mask;

		//a maszk kiterjeszt�se
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(11, 11)));
		
		
		findContours(mask, cont, RETR_EXTERNAL, CHAIN_APPROX_NONE);

		//kihaszn�lva, hogy a sas az egyetlen objektum a k�pen
		r = boundingRect(cont[0]);

		//a k�p r t�glalap �ltal megadott r�sze
		dest = img(r);
		
		//megjelen�t�s
		//imshow("dest", dest);
		//waitKey(33);
		
		//ment�s
		imwrite("sas_" + to_string(fn) + ".png", dest);

		fn += 1;
	}

	return 0;
}