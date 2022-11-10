#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


int main() {  // _noise_filtering

	Mat img = imread("./imgs/szines.png");

	if (img.empty()) {
		cout << "Az kep nem olvashat�." << endl;
		exit(-1);
	}

	imshow("szines", img);

	namedWindow("szuro", WINDOW_NORMAL);
	resizeWindow("szuro", 500, 100);


	int radius = 1;
	createTrackbar("sugar", "szuro", &radius, 50);

	// a sulyozott atlag szamitasanal a kozelebbi pontok es a kozelebbi szinek kapjak az erosebb sulyt,
	// emiatt nagyon magas sigma ertekeknel is tapasztalhato meg valtozas
	//         double gauss_color_coeff = -0.5 / (sigma_color * sigma_color);
	//         double gauss_space_coeff = -0.5 / (sigma_space * sigma_space);

	int sigma_dist = 10;
	createTrackbar("sigma_dist", "szuro", &sigma_dist, 100);    // tavolsag sulyozasahoz 

	int sigma_color = 10;
	createTrackbar("sigma_color", "szuro", &sigma_color, 2000); // szinkulonbseg sulyozasahoz

	int q, ksize = 1;
	int pre_sc = 0, pre_sd = 0, pre_radius = 0;
	Mat dest;

	int first = 1;
	while ((q = waitKey(40)) != 'q') {
		ksize = max(2 * radius + 1, 3);
		sigma_dist = max(sigma_dist, 10);

		if (pre_sd != sigma_dist || pre_sc != sigma_color || pre_radius != radius) {
			// a ksize= -1 a sigma ertekbol szamolja a maszk meretet
			// a sigma_color es a sigma_dist gyakran azonos az egyszeruseg kedveert
			bilateralFilter(img, dest, ksize, sigma_color, sigma_dist);
			imshow("szurt", dest);
			pre_sd = sigma_dist;
			pre_sc = sigma_color;
			pre_radius = radius;
		}
	}

	waitKey(0);

	return 0;
}