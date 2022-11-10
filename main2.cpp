#include "common.h"
#include <opencv2/imgproc.hpp>

#include <vector>

using namespace std;

/// <summary>
/// A img.data bajtsorozat hasznalata. (leggyorsabb eleresi mod, de kenyelmetlen)
/// </summary>
/// <param name="fg"></param>
/// <param name="bg"></param>
void narancsmasolo_szines1(const Mat fg, Mat& bg) {
	assert(fg.isContinuous());

	bg = Mat::zeros(fg.size(), fg.type());

	uchar* p_fg = (uchar*)fg.data;
	uchar* p_bg = (uchar*)bg.data;
	uchar* p_end = p_fg + fg.rows * fg.cols * 3;
	for (; p_fg < p_end; ) {
		if (*(p_fg + 2) > *(p_fg + 1) && *(p_fg + 2) > *p_fg) {
			*p_bg++ = *p_fg++;
			*p_bg++ = *p_fg++;
			*p_bg++ = *p_fg++;
		}
		else {
			p_bg += 3;
			p_fg += 3;
		}
	}
}

/// <summary>
/// Iteratoros megoldas.
/// </summary>
/// <param name="fg"></param>
/// <param name="bg"></param>
void narancsmasolo_szines2(const Mat fg, Mat& bg) {
	bg = Mat::zeros(fg.size(), fg.type());

	cv::MatConstIterator_<Vec3b> fg_it = fg.begin<Vec3b>();	// lustabbaknak ott az auto
	cv::MatConstIterator_<Vec3b> fg_end = fg.end<Vec3b>();
	cv::MatIterator_<Vec3b> bg_it = bg.begin<Vec3b>();
	Vec3b c;
	for (; fg_it != fg_end; ++fg_it, ++bg_it)
	{
		//if ( (*fg_it)[2] > (*fg_it)[1] && (*fg_it)[2] > (*fg_it)[0])
		//    *bg_it = *fg_it;

		// vagy ha kimentjuk a pixelt egy Vec3b-be
		c = *fg_it;
		if (c[2] > c[1] && c[2] > c[0])
			*bg_it = c;
	}
}


/// <summary>
/// For each ciklus. 
/// A koordinatat nem ismerjuk, ezert muszaj egy keppel dolgozni!
/// A pontmasolas helyett klonozni fogjuk az eredeti kepet es toroljuk (feketere allitjuk) a nem kivant pontokat.
/// A matrix tipusos, a hivasnal figyelj!
/// </summary>
/// <param name="fg"></param>
/// <param name="bg"></param>
void narancsmasolo_szines3(const Mat fg, Mat_<Vec3b>& bg) {
	bg = fg.clone();
	Vec3b black(0, 0, 0);
	for (auto& p : bg)
	{
		if (!(p[2] > p[1] && p[2] > p[0])) {
			p = black;
		}
	}
}

///
/// Kizarolag a technika megmutatasara, nem lesz eretelmes a narancsos kepre
///
void masolo_szurke1(const Mat fg, Mat& bg) {
	// szurkeskalassa alakit
	Mat fg_gray;
	cvtColor(fg, fg_gray, COLOR_BGR2GRAY);

	// leteznek olyan keptipusok, ahol pl. 16 bit az egyseg, ezert az utolso oszlopba nem a kephez tartozo
	// pontok kerulhetnek, olyan kepre ne hasznaljuk
	// elvarjuk, hogy ne ilyen kep erkezzen
	assert(fg.isContinuous());

	bg = Mat::zeros(fg.size(), fg_gray.type());

	uchar* p_fg = (uchar*)fg_gray.data;
	uchar* p_bg = (uchar*)bg.data;
	uchar* p_end = p_fg + bg.rows * bg.cols;
	for (; p_fg < p_end; ++p_fg, ++p_bg) {
		if (*p_fg > 150) {
			*p_bg = *p_fg;
		}
	}
}

///
///  iterator 
///
void masolo_szurke2(const Mat fg, Mat& bg) {
	// szurkeskalassa alakit
	Mat fg_gray;
	cvtColor(fg, fg_gray, COLOR_BGR2GRAY);

	bg = Mat::zeros(fg.size(), fg_gray.type());

	cv::MatConstIterator_<uchar> fg_it = fg_gray.begin<uchar>();	// lustabbaknak ott az auto
	cv::MatConstIterator_<uchar> fg_end = fg_gray.end<uchar>();
	cv::MatIterator_<uchar> bg_it = bg.begin<uchar>();
	for (; fg_it != fg_end; ++fg_it, ++bg_it)
	{
		if ((int)(*fg_it) > 150)
			*bg_it = *fg_it;
	}
}


int main() {

	Mat dest1, dest2, destg1, destg2;
	Mat_<Vec3b> dest3;

	Mat img = imread("./imgs/orange1.jpg");
	assert(!img.empty());
	
	imshow("img", img);

	narancsmasolo_szines1(img, dest1);
	imshow("color1", dest1);

	narancsmasolo_szines2(img, dest2);
	imshow("color2", dest2);

	narancsmasolo_szines3(img, dest3);
	imshow("color3", dest3);

	masolo_szurke1(img, destg1);
	imshow("gray1", destg1);

	masolo_szurke2(img, destg2);
	imshow("gray2", destg2);

	waitKey();

	return 0;
}