#include "common.h"

//
// Keppontok vizsgalata 
//

using namespace std;

int main() {

    auto buttons = read_image("./imgs/color_buttons.jpg", IMREAD_COLOR);

    Mat buttonsHSV;
    cvtColor(buttons, buttonsHSV, COLOR_BGR2HSV);

    Mat mask, green;
    inRange(buttonsHSV, Scalar(0,0,0), Scalar(255,10,255), mask);
    mask = 255 - mask;

    buttons.copyTo(green, mask);

    Mat akarmi = Mat::zeros(buttons.size(), buttonsHSV.type());
    copy_img_region(akarmi, green, Point(244, 0), Point(352, 122));

    Mat red(buttons.size(), buttons.type());
    red.setTo(Scalar(0,0,255));

    imshow("red", mask);
    imshow("akarmi", akarmi);

    waitKey(0);

    return 0;
}