//
// Created by 魏 on 2021/1/18.
//

#ifndef PIPEIDENTIFY_TEST_H
#define PIPEIDENTIFY_TEST_H
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
/**
 * 用于测试的类，需要测试的地方请交给Test类进行测试
 * **/
class Test {


public:
    const static cv::String imagePath;

    static void test();
    static void showImage(cv::String windowName, cv::Mat image);

};


#endif //PIPEIDENTIFY_TEST_H
