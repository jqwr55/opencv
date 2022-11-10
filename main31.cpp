/* OpenCV segedanyag
      Konturkezeles
          findContours
*/

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {

	Mat colorImg = imread("./imgs/sajt.png");
	assert(!colorImg.empty());

	Mat gray;
	cvtColor(colorImg, gray, COLOR_BGR2GRAY);

	Mat cheese_mask;
	threshold(gray, cheese_mask, 250, 255, THRESH_BINARY_INV);
	medianBlur(cheese_mask, cheese_mask, 5);

	vector<vector<Point>> contours;
	vector<Vec4i> hier;
	findContours(cheese_mask, contours, hier, RetrievalModes::RETR_TREE, ContourApproximationModes::CHAIN_APPROX_NONE);

	Scalar cheeseColor(0, 255, 0);
	Scalar holeColor(0, 0, 255);

	double cheese = 0;
	double hole = 0;
	for (size_t i = 0; i < hier.size(); ++i) {
		//nincs szulokontur => megvan a sajt
		if (hier[i][3] == -1) {
			drawContours(colorImg, contours, i, cheeseColor, 2);
			cheese += cv::contourArea(contours[i]);
		}
		else {
			drawContours(colorImg, contours, i, holeColor, 2);
			hole += cv::contourArea(contours[i]);
		}
	}

	cout << "A sajtszelet " << round(hole / cheese * 100) << " szazaleka lyuk." << endl;

	imshow("sajtrajz", colorImg);
	waitKey(0);

	return 0;
}