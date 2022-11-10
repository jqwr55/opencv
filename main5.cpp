//
// Padlolapok - szinkulonbseg
//

#include <iostream>
#include <iomanip>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


/// <summary>
/// Egy kep CV_32FC3 tipusra valo konvertalasa.
/// </summary>
/// <param name="img">atalakitando kep</param>
/// <param name="lab">eredmeny kep</param>
void convert(const Mat img, Mat& lab) {
	Mat imgf;
	img.convertTo(imgf, CV_32FC3, 1.0 / 255.0);
	cvtColor(imgf, lab, COLOR_BGR2Lab);
}

/// <summary>
/// Ket kep teljes, atlagos szinkulonbsege.
/// </summary>
/// <param name="etalon_lab">Lab szinterben levo kep</param>
/// <param name="img_lab">Lab szinterben levo kep</param>
/// <returns></returns>
double compareImg(const Mat etalon_lab, const Mat img_lab) {
	
	Mat diff = etalon_lab - img_lab;  //L1-L2, a1-a2, b1-b2
	
	Mat diff2 = diff.mul(diff); // (L1-L2)^2 + (a1-a2)^2 + (b1-b2)^2
	
	vector<Mat> chs;
	split(diff2, chs);
	//
	Mat dist;
	cv::sqrt(chs[0] + chs[1] + chs[2], dist); //sqrt ((L1-L2)^2 + (a1-a2)^2 + (b1-b2)^2) 

	return cv::mean(dist)[0];
}

// vagy nincs split �s 1 csatorn�s k�pp form�lja 
//cv::transform(diff, diff, cv::Mat::ones(1, 3, CV_32F)); // minden pontra  [B, G, R] * [1, 1, 1].t() 
// cv::sqrt(diff, diff);

/// <summary>
/// Szinkulonbseg alapjan a kategoria visszaadasa.
/// </summary>
/// <param name="d">a teljes szinkulonbseg erteke</param>
/// <returns></returns>
string getCategory(double d) {
	assert(d >= 0);

	if (d < 0.5) 
		return "Szemmel nem lathato";
	if (d < 1.5) 
		return "Alig erzekelheto";
	if (d < 3.0) 
		return "Erzekelheto";
	if (d < 6.0)
	    return "Jol lathato";
	
	return "Nagy szininger kulonbseg";
}


int main() {

	Mat etalon = imread("./imgs/etalon.png");
	imshow("etalon", etalon);

	Mat etalon_lab;
	convert(etalon, etalon_lab);

	Mat img, img_lab;
	
	for (int i = 1; i <= 7; ++i) {
		// ha std::format-ot akar hasznalni, akkor a Projekt beallitasoknal 
		// a C++ Language Standard �t�ll�tand� 20-asra vagy �jabbra.

		img = imread("./imgs/fa_" + to_string(i) + ".png");
		if (img.empty())
			continue;

		string wname = "img_" + to_string(i);
		imshow(wname, img);
		moveWindow(wname, 200, i * 80);

		convert(img, img_lab);

		double d = compareImg(etalon_lab, img_lab);
		string cat = getCategory(d);

		cout << "fa_" << i << ": " << d << " " << cat << endl;
		
	}

	waitKey(0);
	return 0;
}