#include <iostream>

#define HAVE_OPENCV_IMGPROC
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/hal/interface.h>

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