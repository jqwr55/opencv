#include "common.h"

//
// Keppontok vizsgalata 
//

using namespace std;

/// <summary>
/// Atmeretezi a hatteret az eloter meretere, es atmasolja Kuramat a modositott hatterre.
/// </summary>
/// <param name="fg"></param>
/// <param name="bg"></param>
void eloterMasolo1(const cv::Mat fg, cv::Mat& bg) {
    cv::resize(bg, bg, fg.size());
    cv::Vec3b c;
    //cv::Vec3b white(255, 255, 255);
    for (int i = 0; i < fg.rows; ++i) {
        for (int j = 0; j < fg.cols; ++j) {
            c = fg.at<cv::Vec3b>(i, j);
            // ez az, ami csak a tiszta feher pontokat hagyna ki
            //if (c != white)
            
            // ezzel mar csak akkor masolunk, ha viszonylag eros elteres van a fehertol
            if (c[0] < 230 || c[1] < 230 || c[2] < 230 )
                bg.at<cv::Vec3b>(i, j) = fg.at<cv::Vec3b>(i, j);
        }
    }
}

/// <summary>
/// Atmeretezi a hatteret az eloter meretere, es atmasolja Kuramat a modositott hatterre. 
/// </summary>
/// <param name="fg"></param>
/// <param name="bg"></param>
void eloterMasolo2(const cv::Mat fg, cv::Mat& bg) {
    cv::resize(bg, bg, fg.size());

    // a nem feher pontok kijelolese fuggvennyel
    // csak akkor masol, ha a B es G csatornan 0-240 kozott es az R-en 0-250 kozott van a pixelertek.  
    Mat mask;
    inRange(fg, Scalar(0, 0, 0), Scalar(240, 240, 250), mask);

    // ha a copyTo fuggvenytnek atadunk egy 2. kepet is, akkor csak azon pontok masolodnak, amely azon nem 0 erteku
    fg.copyTo(bg, mask);
}

/// <summary>
/// Megfelelo szinu pontok masolasa pixelenkent.
/// </summary>
/// <param name="rgb"></param>
/// <param name="dest"></param>
void narancsMasoloRGB(const cv::Mat rgb, cv::Mat& dest) {
    dest.create(rgb.size(), CV_8UC3);
    dest.setTo(0);

    cv::Vec3b c;
    for (int i = 0; i < rgb.rows; ++i) {
        for (int j = 0; j < rgb.cols; ++j) {
            c = rgb.at<cv::Vec3b>(i, j);
            // a B, G, R ertekeket hasonlithatjuk konstans ertekekhez
            // de egymassal is osszevethetjuk oket
            // az alabbi keplet csak azt varja el, hogy a piros legyen erosebb, mint a tobbi
            if (c[2] > c[1] && c[2] > c[0])
                dest.at<cv::Vec3b>(i, j) = rgb.at<cv::Vec3b>(i, j);
        }
    }
}

/// <summary>
/// A megfelelo szinu pixelek atmasolasa egy uj kepre. Tipusos matrix.
/// </summary>
/// <param name="rgb"></param>
/// <param name="dest"></param>
void narancsMasoloHSV(const cv::Mat_<cv::Vec3b> rgb, cv::Mat_<cv::Vec3b>& dest) {
    Mat_<Vec3b> hsv;  //csak egy fejlecet kell letrehozni, mert a cvtColor 2. parametere OutputArray
    cvtColor(rgb, hsv, ColorConversionCodes::COLOR_BGR2HSV);

    //a dest tipusa mar rogzitett, ezert csak a meretet adjuk meg
    dest.create(rgb.size());
    dest.setTo(0);

    int hue;
    for (int i = 0; i < rgb.rows; ++i) {
        for (int j = 0; j < rgb.cols; ++j) {
            //a matrix tipus "beegetese" miatt egyszerusodik a pontok elerese
            hue = hsv(i, j)[0];

            //megj: eredemes lehet a nagyon alacsony telitetsegu pontokat is kidobni, ha szurke kerul az eredmenybe
            if (hue < 30) { //&& hsv(i, j)[1] > 5
                // az eredeti rgb keprol masoljuk at a pontokat
                // a hsv-t csak a pontok szinenek ellenorzesehez hasznaljuk
                dest(i, j) = rgb(i, j);
            }
        }
    }
}

/// <summary>
/// Masolas beepitett fuggvenyek hasznalataval. 
/// a) masolando resz maszkjanak letehozasa
/// b) maszk alatti resz masolasa
/// </summary>
/// <param name="rgb"></param>
/// <param name="dest"></param>
void narancsMasoloHSV2(const cv::Mat rgb, cv::Mat& dest) {

    Mat hsv;
    cvtColor(rgb, hsv, ColorConversionCodes::COLOR_BGR2HSV);
    // a narancs szinu pontok "kijelolese"  
    Mat mask;
    inRange(hsv, Scalar(0, 0, 0), Scalar(30, 255, 255), mask);
    // a maszk feher resze ala eso keppontok masolasa az eredeti rgb keprol az eredmeny kepre
    rgb.copyTo(dest, mask);
}

int main() {

    Mat fg = imread("./imgs/kurama.jpg");
    Mat bg = imread("./imgs/background.jpg");
    if (fg.empty() || bg.empty())
    {
        cout << "A kep nem talalhato" << endl;
        exit(-1);
    }

    Mat bg2 = bg.clone(); // mert az eloterMasolo modositja majd a bg-t
    eloterMasolo1(fg, bg);
    imshow("osszemasolt1", bg);

    eloterMasolo2(fg, bg2);
    imshow("osszemasolt2", bg2);

    Mat orig = imread("./imgs/orange1.jpg");
    if (orig.empty())
    {
        cout << "A kep nem talalhato" << endl;
        exit(-1);
    }

    Mat dest_rgb;
    narancsMasoloRGB(orig, dest_rgb);

    Mat_<Vec3b> dest_hsv;
    narancsMasoloHSV(orig, dest_hsv);

    Mat dest_hsv2;
    narancsMasoloHSV2(orig, dest_hsv2);

    imshow("eredeti", orig);
    imshow("masolt_rgb", dest_rgb);
    imshow("masolt_hsv", dest_hsv);
    imshow("masolt_hsv2", dest_hsv2);
    waitKey(0);

    return 0;
}