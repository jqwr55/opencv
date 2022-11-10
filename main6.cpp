#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void convert(const Mat rgb, vector<Mat>& dest) {
	Mat rgbf;
	rgb.convertTo(rgbf, CV_32FC3, 1 / 255.0);

	Mat labf;
	cvtColor(rgbf, labf, COLOR_BGR2Lab);

	cv::split(labf, dest);
}

int main() {

	Mat etalon = imread("./imgs/etalon.png");
	imshow("etalon", etalon);

	vector<Mat> echannels ;
	convert(etalon, echannels);
	
	for (int i = 1; i <= 7; ++i){
		string path = "./imgs/fa_" + std::to_string(i) + ".png";

		Mat rgb = imread(path);
		
		vector<Mat> channels;
		convert(rgb, channels);

		Mat dL = channels[0] - echannels[0];
		Mat da = channels[1] - echannels[1];
		Mat db = channels[2] - echannels[2];
		Mat dE;
		cv::sqrt( dL.mul(dL) + da.mul(da) + db.mul(db), dE );
		
		Scalar avg = mean(dE);

		imshow("rgb", rgb);
		//waitKey(0);


		std::cout << "atlag: " << avg << endl;

		if (avg[0] < 0.5)
			std::cout << "nem erzekelheto" << endl;
		if (0.5 <= avg[0] && avg[0] < 1.5)
			std::cout << "alig erzekelheto" << endl;
		if (1.5 <= avg[0] && avg[0] < 3)
			std::cout << "erzekelheto" << endl;
		if (3.0 <= avg[0] && avg[0] < 6.0)
			std::cout << "jol lathato" << endl;
		if (6.0 <= avg[0] )
			std::cout << "nagy kulonbseg" << endl;


    }


	return 0;
}