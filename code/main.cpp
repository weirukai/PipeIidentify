#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include "GeneralProcessor.h"
#include "Test.h"
using namespace std;
using namespace cv;

//   main 部分，只能调用函数，不可在main中工作
int main() {
    Test::test();
    return 0;
}
