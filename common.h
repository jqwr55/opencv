#pragma once

#include <iostream>
#include <vector>

#define HAVE_OPENCV_IMGPROC
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/hal/interface.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat read_image(const char* path, ImreadModes mode) {

    auto ret = imread(path, mode);
    if(ret.empty()) {

        std::cout << "No image data" << std::endl;
        return {};
    }

    return ret;
}
void print_image_info(Mat img) {

    auto channelCount = (img.type() >> CV_CN_SHIFT) + 1;
    auto depth = img.type() & CV_MAT_DEPTH_MASK;

    const char* DEPTH_STRS[] = {
        "U8",
        "S8",
        "U16",
        "S16",
        "S32",
        "F32",
        "F64",
        "F16",
    };

    std::cout <<
        "image info {\n" <<
        "\tsize:\t" << img.size << '\n' <<
        "\ttype:\t" << DEPTH_STRS[depth] << "[" << channelCount << "]\n" <<
        "}\n";
}

void copy_img_region(Mat& dst, const Mat src, Point leftTop, Point rightBottom) {

    for(int y = leftTop.y; y <= rightBottom.y; y++) {

        for(int x = leftTop.x; x <= rightBottom.x; x++) {

            dst.at<Vec3b>(Point(x, y)) = src.at<Vec3b>( Point(x, y) );
        }
    }
}

void megjelenit(const string ablak, const Mat img) {
	Mat gviz;
	convertScaleAbs(img, gviz);
	imshow(ablak, gviz);
}


namespace Histo {
	void calcHistoC1(const cv::Mat img, cv::Mat& histo);
	void calcHistoC3(const cv::Mat img, vector<cv::Mat>& histo);

	void drawHistoC1(const cv::Mat histo, cv::Mat& canvas, int channel = -1);
	void drawHistoC3(const vector<cv::Mat> histos, vector<cv::Mat>& canvases);

	/// <summary>
	/// Egy kep hisztogramjanak megjelenitese. 
	/// </summary>
	/// <param name="img">1 vagy 3 csatornas input kep</param>
	/// <param name="title">ablaknev</param>
	/// <param name="wait">varakoztatas</param>
	void showHisto(Mat img, string title = "histo", int wait = 0);
};

/*
Kepfeld. gyak. - nehany segedfuggveny az orai anyag konnyebb szemleltetesehez.
*/

void Histo::calcHistoC1(const cv::Mat img, cv::Mat& histo) {
	assert(img.type() == CV_8UC1);

	// CV_32U nem letezik, ha de CV_16U-t (ushort) hasznalhatsz, ha akarsz.
	histo = Mat::zeros(256, 1, CV_32S);
	for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++)
			histo.at<int>(img.at<uchar>(i, j))++;
}


void Histo::calcHistoC3(const cv::Mat img, vector<cv::Mat>& histo_vect) {
	assert(img.type() == CV_8UC3);

	vector<Mat> chs;
	split(img.clone(), chs);

	Mat h0, h1, h2;
	Histo::calcHistoC1(chs[0], h0);
	Histo::calcHistoC1(chs[1], h1);
	Histo::calcHistoC1(chs[2], h2);

	histo_vect.clear();
	histo_vect.push_back(h0);
	histo_vect.push_back(h1);
	histo_vect.push_back(h2);
}



void Histo::drawHistoC1(const cv::Mat histo, cv::Mat& canvas, int channel) {
	assert(histo.type() == CV_32S);

	int pad = 10;
	canvas = Mat::zeros(200 + 2 * pad, 256 + 2 * pad, CV_8UC3);
	
	// a hisztogram alatti szurke sav rajzolasa 
	for (int i = 0; i < 10; ++i) {  // 10 pixel vastag lesz, hogy jol latszodjon
		for (int j = 0; j < 256; ++j) {
			if (0 <= channel && channel <= 2) {
				canvas.at<Vec3b>(canvas.rows - 1 - i, j + pad)[channel] = j;
			}
			else {
				canvas.at<Vec3b>(canvas.rows - 1 - i, j + pad) = Vec3b(j, j, j);
			}
		}
	}
	

	Vec3b color(255, 255, 255);
	if (0 <= channel && channel <= 2) {
		color = Vec3b(0, 0, 0);
		color[channel] = 255;
	}


	double minv, maxv;
	minMaxLoc(histo, &minv, &maxv);
	
	double scale = 200 / maxv;  // az ablak magassagahoz igazitom a hisztogram magassagat

	// a hisztogram rajzolasa
	for (int i = 0; i < 256; ++i) {
		// egy oszlop magassage (a line(Point(0, 0), Point(0, 1)) 2 pixelt rajzolna, ezert a -1)
		int v = int(histo.at<int>(i) * scale) - 1; 
		// canvas.rows-2-pad oldja meg, hogy a szurke sav felett kezdjuk a rajzolast
		if (v >= 0)
			line(canvas, Point(pad + i, canvas.rows - 2 - v - pad), Point(pad + i, canvas.rows - 2 - pad), color);
	}
}


void Histo::drawHistoC3(const vector<cv::Mat> histos, vector<cv::Mat>& canvases) {
	assert(histos.size() == 3 && 
		   histos[0].type() == CV_32S && histos[1].type() == CV_32S && histos[2].type() == CV_32S);

	canvases.clear();
	for (int i = 0; i <= 2; ++i) {
		Mat canvas;
		Histo::drawHistoC1(histos[i], canvas, i);
		canvases.push_back(canvas.clone());
	}
}


void Histo::showHisto(Mat img, string title, int wait) {

	assert(img.type() == CV_8UC1 || img.type() == CV_8UC3);
	if (img.type() == CV_8UC1) {
		Mat histo;
		Histo::calcHistoC1(img, histo);

		// az ertekek konzolra iratasa
		//cout << histo << endl;

		Mat canvas;
		Histo::drawHistoC1(histo, canvas);
		imshow(title, canvas);
	}
	else if (img.type() == CV_8UC3) {
		vector<Mat> histos;
		Histo::calcHistoC3(img, histos);

		vector<Mat> canvases;
		Histo::drawHistoC3(histos, canvases);

		string str = "BGR";
		for (int i = 0; i <= 2; ++i) {
			imshow(title + "-" + str[i], canvases[i]);
		}
	}

	waitKey(wait);
}

void equalizeHistColor1(const Mat img, Mat& dest) {

	vector<Mat> chs;
	split(img.clone(), chs);
	equalizeHist(chs[0], chs[0]);
	equalizeHist(chs[1], chs[1]);
	equalizeHist(chs[2], chs[2]);
	merge(chs, dest);
}

void equalizeHistColor2(const Mat img, Mat& dest) {

	Mat lab;
	cvtColor(img, lab, COLOR_BGR2Lab);

	vector<Mat> chs;
	split(lab, chs);

	equalizeHist(chs[0], chs[0]);

	Mat dest_lab;
	merge(chs, dest_lab);

	cvtColor(dest_lab, dest, COLOR_Lab2BGR);
}

void createHisto(const cv::Mat img, cv::Mat& histo) {

	vector<Mat> kepek;       
	kepek.push_back(img); // egy k�pet haszn�lunk
	
	vector<int> csatornak;         
	csatornak.push_back(0); // annak az egy k�pnek a 0. csatorn�j�t haszn�ljuk
	
	vector<int> hiszto_meretek; 
	hiszto_meretek.push_back(256);  //minden vil�goss�gk�dot k�l�n sz�molunk

	vector<float> hiszto_tartomanyok;
	hiszto_tartomanyok.push_back(0.0f); //hol kezd�dik a tartom�ny
	hiszto_tartomanyok.push_back(255);  //meddig tart
   
	//accumlate: marad false (null�zza a hisztogrammot)
	calcHist(kepek, csatornak, noArray(), histo, hiszto_meretek, hiszto_tartomanyok, false);
}

int calc_th_value(const Mat src, float fg_ratio = 1.0f) {

	Mat histo;
	createHisto(src, histo);

	float osszes = src.cols * src.rows; //összes képpont száma
	float sum = 0;
	int th = 0;

	for (int i = 0; i < histo.rows; ++i) {
		sum += histo.at<float>(i);
		if ( (sum / osszes) > fg_ratio ) {
			return i;
		}
	}

	return 255;
}
void roberts(const Mat img, Mat& dest) {
	Mat tmp = Mat::zeros(img.size(), CV_16S);

	// ezzel megsporolhato a .at<uchar> resz kiirasa a pixelertekek lekeresekor
	Mat_<uchar> img2 = (Mat_<uchar>)img;

	for (int i = 0; i < img.rows-1; ++i) {
		for (int j = 0; j < img.cols-1; ++j) {
			//a b
			//c d
			//gradiens errosseg kozelitese: |a-d| + |b-c|
			tmp.at<short>(i, j) = fabs(img2(i, j) - img2(i + 1, j + 1)) + fabs(img2(i, j + 1) - img2(i + 1, j));
		}
	}
	//a lathatosag es a kompaktabb tarolas erdekeben visszaterunk a szokasos vilagba
	convertScaleAbs(tmp, dest);

	//egyebken egy pici szorzassal is meg lehetne oldani, hogy lathato legyen az eredmeny
	//a kozepszurke ebben az esetben a 0-t jelenti, mert elojeles tipusunk (CV_16S, short) van.
	//dest = tmp.clone()*255;
}

/// <summary>
/// Gradiens erosseg kozelitese 
/// </summary>
/// <param name="img"></param>
/// <param name="dest"></param>
void sobel_approx(const Mat img, Mat& dest) {
	Mat gx, gy, tmp;
	Sobel(img, gx, CV_16S, 1, 0);
	Sobel(img, gy, CV_16S, 0, 1);
	tmp = cv::abs(gx) + cv::abs(gy);
	convertScaleAbs(tmp, dest);
}

/// <summary>
/// Gradiens erosseg szamitasa
/// Eroforrasigenyesebb, mint a kozelites, ezert sokkal ritkabban hasznaljuk.
/// </summary>
/// <param name="img"></param>
/// <param name="dest"></param>
void sobel(const Mat img, Mat& dest) {
	Mat gx, gy, tmp;
	// a gyokvonas miatt float-ot hasznalunk a tarolasra!
	Sobel(img, gx, CV_32F, 1, 0);
	Sobel(img, gy, CV_32F, 0, 1);
	cv::sqrt(gx.mul(gx)+gy.mul(gy), tmp);
	convertScaleAbs(tmp, dest);
}

void SobelElek(const Mat img, Mat& dest) {
	//Deriv�lt meghat�roz�sra
	Mat gx, gy, g;
	Sobel(img, gx, CV_16S, 1, 0);  // fuggoletes iranyu elek kiemelese
	Sobel(img, gy, CV_16S, 0, 1);  // vizszintes iranyu elek kiemelese 
	g = abs(gx) + abs(gy);  //a deriv�lt

    //B�jtra konvert�l
	convertScaleAbs(g, g);

	//Automatikus k�sz�b�l�s
	threshold(g, dest, 0, 255, THRESH_OTSU);
}


///
/// �ldetekt�l�s Canny oper�torral
///
void CannyElek(const Mat img, Mat& dest, int th1) {
	assert(th1 * 3 < 255);
	Canny(img, dest, th1, th1*3 );
}

void kordetektalo1(const Mat& gray, vector<Vec3f>& circles) {

	int cannyTh = 80;  // biztos el hatara a Canny detektornak
	int minPoints = 50; // hany eloterponton kell atmennia a korvonalnak
	int minR = 22;  //minimalis sugar
	int maxR = 26;  //maximalis sugar
	int d = 2; //felbontast osztja (1-> eredeti, 2 -> 2x2-es resz egy 'doboz', stb)  
	int minDist = 10; //a minR-nel kozelebb korokbol az erosebbet kerjuk

	//vector<Vec3f> circles; // Vec3f: [0]: kozeppont x,  [1] kozeppont y, [2] subar
	cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, d, minDist, cannyTh, minPoints, minR, maxR);
}

/// <summary>
/// Scharr elkeresore epit (pontosabban becsulheto az el irany es erosseg)
/// </summary>
/// <param name="gray"></param>
/// <param name="circles"></param>
void kordetektalo2(const Mat& gray, vector<Vec3f>& circles) {
	int cannyTh = 80;  // biztos el hatara a Canny detektornak, de Sobel helyett Scharr operatort hasznal
	double perfectness = 0.70; // mennyire legyen tokeletes kor
	int minR = 20;  //minimalis sugar
	int maxR = 28;  //maximalis sugar
	int d = 1; //felbontast osztja (1-> eredeti, 2 -> 2x2-es resz egy 'doboz', stb)  
	int minDist = 10; //a minR-nel kozelebb korokbol az erosebbet kerjuk

	//vector<Vec3f> circles; // Vec3f: [0]: kozeppont x,  [1] kozeppont y, [2] subar
	cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT_ALT, d, minDist, cannyTh, perfectness, minR, maxR);
}