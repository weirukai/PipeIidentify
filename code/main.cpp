#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;






int main() {

    Mat image = imread("D:\\ClCodes\\PipeIdentify\\images\\1.jpg", IMREAD_ANYCOLOR);
    imshow("test", image);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
