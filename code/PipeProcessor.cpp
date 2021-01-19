//
// Created by κ on 2021/1/18.
//

#include "PipeProcessor.h"
#include <vector>

Mat PipeProcessor::getPipeByColor(Mat image) {
    Mat hsvMask;
    std::vector<Mat> hsvChannels;
    cvtColor(image, image, COLOR_RGB2HSV);
    split(image, hsvChannels);
    inRange(image, Scalar(0, 0, 0), Scalar(180, 60, 255), hsvMask);
    bitwise_not(hsvMask,hsvMask);
    // 闭运算填充小孔
    Mat kernel=getStructuringElement(MORPH_RECT,Size(16,16));
    morphologyEx(hsvMask,hsvMask,MORPH_CLOSE,kernel);
    bitwise_not(hsvMask,hsvMask);
//    kernel=getStructuringElement(MORPH_RECT,Size(1,1));
//    morphologyEx(hsvMask,hsvMask,MORPH_OPEN,kernel);
    kernel=getStructuringElement(MORPH_RECT,Size(16,16));
    morphologyEx(hsvMask,hsvMask,MORPH_CLOSE,kernel);
    return hsvMask;
}

Mat PipeProcessor::getPipeByThreshold(class cv::Mat image) {
    cvtColor(image,image,COLOR_RGB2GRAY);
    threshold(image,image,10,255,CV_THRESH_OTSU);
    return image;
}