//
// Created by 魏 on 2021/1/18.
//

#include "GeneralProcessor.h"
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <vector>

using namespace cv;

/**
 * 预处理，提升亮度，去除部分噪声
 * */
cv::Mat GeneralProcessor::preProcess(cv::Mat image) {
    std::vector<Mat> channels;
    Mat image_V;//明度空间
    Mat imageBak = image.clone();
    GaussianBlur(image, image, Size(5, 5), 0);
    cvtColor(image, image, COLOR_RGB2HSV);
    split(image, channels);
    channels[2]=channels.at(2)+40;
    merge(channels,image);
    cvtColor(image, image, COLOR_HSV2RGB);
    showImage("V",image);


}

Mat GeneralProcessor::showImage(cv::String windowName, cv::Mat image) {
    /**
     * 用来显示图片的部分，用以赋值到其他地方
     * */
    namedWindow(windowName,0);
    resizeWindow(windowName,640,512);
    imshow(windowName, image);
    waitKey(0);
    destroyAllWindows();
}