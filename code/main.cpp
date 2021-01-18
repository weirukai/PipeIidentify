#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include "GeneralProcessor.h"

using namespace std;
using namespace cv;

//   main 部分，只能调用函数，不可在main中工作
int main() {

    Mat image = imread("D:\\ClCodes\\PipeIdentify\\images\\1.jpg", IMREAD_ANYCOLOR);
    GeneralProcessor::preProcess(image);


    return 0;
}
