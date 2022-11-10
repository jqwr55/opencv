#ifndef _my_util_cpp
#define _my_util_cpp

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <atomic>
#include <iostream>
#include <map>
#include <tuple>
namespace Resolution { //TODO cant believe this hack works

#if defined(_WIN32)
#include <windows.h>
#else
#include <X11/Xlib.h>
#endif

	//https://stackoverflow.com/questions/11041607/getting-screen-size-on-opencv
	void getScreenResolution(int &width, int &height) {
#if defined(_WIN32)
		width = (int)GetSystemMetrics(SM_CXSCREEN);
		height = (int)GetSystemMetrics(SM_CYSCREEN);
#else
		Display* disp = XOpenDisplay(NULL);
		Screen*  scrn = DefaultScreenOfDisplay(disp);
		width = scrn->width;
		height = scrn->height;
#endif
	}

}

namespace mUtil {
	using namespace cv;

	static bool mmydebug = true;
#define DEBUG(X) do { if(mmydebug) { X } } while(0)

	static std::atomic<int> __mycounter = 0;
	//TODO enlarge if too small?
	void mshow(const cv::Mat &img, string name = "",  bool enlarge_small = true) {
		if (!mmydebug) return;

		Mat tmp;
		img.copyTo(tmp);
		int w, h;
		Resolution::getScreenResolution(w, h);
		double ratio = 1.0;
		// size to at most 1/4th of the screen
		ratio = min(1.0, min(1.0 / (img.rows / (h / 2.0)), 1.0 / (img.cols / (w / 2.0))));
		if (enlarge_small && (img.rows / h < 1.0 / 4.0 || img.cols / w < 1.0 / 6.0)) {
			// make it bigger if it's really small
			ratio = min((h / 2.0 / img.rows), (w / 2.0 / img.cols)); // TODO I didn't really think through this, probably broken.
		}
		resize(tmp, tmp, Size(), ratio, ratio, INTER_CUBIC);
		cv::imshow(
			(name != "") ? name : std::to_string(++__mycounter),
			tmp);
	}

	//checked read
	Mat cread(std::string path, int flags) {
		Mat res = imread(path, flags);
		if (res.empty()) {
			std::cout << "Upsz" << std::endl;
			exit(-1);
		}
		return res;
	}

	VideoCapture cread_video(string path) {
		VideoCapture cap(path);
		double frcnt = cap.get(CAP_PROP_FRAME_COUNT); //TODO technically this is a double?
		printf("rate: %f, frames: %f\n", cap.get(CAP_PROP_FPS), frcnt);
		if (!cap.isOpened() || frcnt == 0) {
			cout << "Upsz" << endl;
			exit(-1);
		}
		return cap;
	}

	//TODO map/elements should be const?
	void tracker(const Mat img, map<string, int> &vals, std::function<void(Mat&, const Mat&, map<string, int>&)> mkDisp, int counts = 255, bool enable = true) {

		if (!mmydebug || !enable) return;
		string ttt;
		string* thetext = & ttt;
		Mat* theimg = 0;
		auto tup = make_tuple(&thetext, &theimg); //TODO bleh this is a mess

		namedWindow("tracker");
		setMouseCallback("tracker", [](int event, int x, int y, int flags, void *userdata){
			auto tup = *((tuple<string**,Mat**>*)userdata); //TODO ugh pointer syntax
			string* thetext = *get<string**>(tup);
			Mat* theimg = *get<Mat**>(tup);
			vector<Mat> vec;
			if (theimg) {
				split(*theimg, vec);//stupid hacks
				*thetext = "(y" +std::to_string(y) + ",x" + std::to_string(x) + ") || (b"
					+ to_string(vec[0].at<uchar>(y, x)) + ",g"
					+ ((vec.size() > 1) ? (to_string(vec[1].at<uchar>(y, x)) + ",r"
						+ to_string(vec[2].at<uchar>(y, x)) + ")") : ""); //TODO type specificity hack issues
			}}, &tup);
		mshow(img, "tracker");
		for (auto& v : vals) {
			createTrackbar(v.first, "tracker", &(v.second), counts, nullptr, nullptr);
		}
		Mat disp;
		while (1) {
			mkDisp(disp, img, vals);
			theimg = &disp;
			if (thetext) {
				putText(disp, *thetext, Point(30, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,0));
			}
			mshow(disp, "tracker");
			char key = (char)waitKey(10);
			if (key == 'q') {
				for (auto v : vals) {
					printf("%s: %d, ", v.first.c_str(), v.second);
				}
				printf("\n");
				break;
			}
		}
	}

}
#endif