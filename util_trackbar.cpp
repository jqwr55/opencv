/*
OpenCV segadanyagok:
Pelda morfologiai szurore
*/

#include <iostream>
#include <opencv2/core.hpp>    // osztalyok, strukturak, alapveto funkciok
#include <opencv2/highgui.hpp> // felulet
#include <opencv2/imgproc.hpp> // kepfeld. fuggvenyek
#include "common.h" // kepfeld. fuggvenyek
#include "util.hpp" // kepfeld. fuggvenyek
#include <map> // kepfeld. fuggvenyek

using namespace std;
using namespace cv;
using namespace mUtil;

void lambda(Mat& dst, const Mat& img, map<string, int>& vals) {

    auto tmp = img.clone();
    
    auto kernel = vals["asd"];
    kernel = max(10, min(kernel, 1)) * 2 + 1;
    medianBlur(tmp, tmp, kernel);
    dst = tmp;
}

int main() {

    auto sajt = cread("./imgs/sajt.png", IMREAD_GRAYSCALE);

    map<string, int> t = { {"asd", 5} };
    tracker(sajt, t, lambda);

	waitKey(0);
}