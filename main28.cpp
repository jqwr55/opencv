///
/// Kepfeld segedanyag.
/// 

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;



void fill_holes(const Mat& binary_img, Mat& dest, int radius) {
	Mat tmp, dilated;
	copyMakeBorder(binary_img, tmp, radius, radius, radius, radius, BORDER_CONSTANT, Scalar(0, 0, 0));

	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(2 * radius + 1, 2 * radius + 1));
	dilate(tmp, dilated, kernel);
	erode(dilated, dest, kernel);

	dest = dest(Rect(radius, radius, binary_img.cols, binary_img.rows)).clone();
}

void fill_holes2(const Mat& binary_img, Mat& dest, int radius) {
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(2 * radius + 1, 2 * radius + 1));
	cv::morphologyEx(binary_img, dest, MORPH_CLOSE, kernel, Point(-1, -1), 1, BORDER_CONSTANT, 0);
}

/// <summary>
/// Elek: dilatalt-eredeti
/// </summary>
/// <param name="binary_img"></param>
/// <param name="dest"></param>
void borders(const Mat& binary_img, Mat& dest) {
	cv::dilate(binary_img, dest, getStructuringElement(MORPH_RECT, Size(3, 3)));
	dest = dest - binary_img;
}

/// <summary>
/// Elek: eredeti-erodalt
/// </summary>
/// <param name="binary_img"></param>
/// <param name="dest"></param>
void borders2(const Mat& binary_img, Mat& dest) {
	cv::erode(binary_img, dest, getStructuringElement(MORPH_RECT, Size(3, 3)));
	dest =  binary_img - dest;
}

/// <summary>
/// Megj.: kihasznaltuk, hogy a kepen csak a kereszt alakoku jeloleseknel "illeszkedik" a MORPH_CROSS
/// Ha a MORPH_CROSS-nal nagyobb foltok lennenek, akkor azok is megmaradnanak a kepen.
/// </summary>
/// <param name="binary_img"></param>
/// <param name="dest"></param>
void findCrosses(const Mat& binary_img, vector<Point>& dest) {
	Mat tmp;
	// jelen esetben hasznalhatunk eroziot
	// cv::erode(binary_img, tmp, getStructuringElement(MORPH_CROSS, Size(3, 3)));
	
	//vagy hasznalhatjuk a kimondottan binaris mintak illesztesere szolgalo HIT and MISS operator is
	cv::morphologyEx(binary_img, tmp, MORPH_HITMISS, getStructuringElement(MORPH_CROSS, Size(3, 3)));

	for (int i = 0; i < tmp.rows; ++i) {
		for (int j = 0; j < tmp.cols; ++j) {
			if (tmp.at<uchar>(i, j))
				dest.push_back(Point(j, i));
		}
	}
}

void sajtos_feladatok(const string& filename = "swiss_cheese.jpg") {

	Mat cheese = imread(filename, IMREAD_GRAYSCALE);
	Mat cheese_bin;
	threshold(cheese, cheese_bin, 250, 255, THRESH_BINARY_INV);
	medianBlur(cheese_bin, cheese_bin, 5);

	imshow("cheese", cheese);
	imshow("cheese_bin", cheese_bin);

	Mat more_cheese;
	fill_holes(cheese_bin, more_cheese, 40);
	imshow("more_cheese", more_cheese);

	Mat more_cheese2;
	fill_holes2(cheese_bin, more_cheese2, 40);
	imshow("more_cheese2", more_cheese2);

	Mat contoursImg;
	borders(cheese_bin, contoursImg);
	imshow("borders", contoursImg);

	Mat contoursImg2;
	borders(cheese_bin, contoursImg2);
	imshow("borders2", contoursImg2);

	waitKey(0);
}

void sejtes_feladat(const string& filename = "annotalt_sejt.png") {
	Mat cells = imread(filename);

	imshow("cells", cells);

	vector<Mat> chs;
	cv::split(cells, chs);
	Mat cells_bin;
	threshold(chs[2], cells_bin, 250, 255, THRESH_BINARY);

	vector<Point> pts;
	findCrosses(cells_bin, pts);

	// kep rajzolasa
	Mat cells2 = cells.clone();
	for (auto p : pts) {
		drawMarker(cells2, p, Scalar(0, 255, 0), MARKER_CROSS);
	}

	imshow("markers", cells2);
	waitKey(0);
}

int main() {

	sajtos_feladatok("./imgs/sajt.png");

	return 0;
}