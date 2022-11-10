#include "common.h"

int main(int argc, char** argv ) {

    auto image = read_image("./imgs/kurama.jpg", IMREAD_COLOR);
    print_image_info(image);

    imshow("Display Image", image);
    waitKey(0);


    return 0;
}