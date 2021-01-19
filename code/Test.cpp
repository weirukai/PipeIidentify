//
// Created by κ on 2021/1/18.
//

#include "Test.h"
#include "PipeProcessor.h"
#include "GeneralProcessor.h"
const cv::String Test::imagePath = "D:\\ClCodes\\PipeIdentify\\images\\508.jpg";

/**
 * 为保证main函数的结构清晰，测试需要在这里进行
 * 除main外，其他文件中不应该调用test测试类
 * */
void Test::test() {
    /*******
     *
     * 测试处，请在此处进行相应的测试
     *
     * ******/
    Mat image = imread(Test::imagePath, IMREAD_ANYCOLOR);
    Mat imageBak=image.clone();
    image=GeneralProcessor::preProcess(image);
    image = PipeProcessor::getPipeByThreshold(image);
    imageBak.copyTo(image,image);
    showImage("threshold", image);
}

void Test::showImage(cv::String windowName, cv::Mat image) {

    namedWindow(windowName, 0);
    resizeWindow(windowName, 640, 512);
    imshow(windowName, image);
    waitKey(0);
    destroyAllWindows();
}