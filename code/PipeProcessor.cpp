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
    bitwise_not(hsvMask, hsvMask);
    // 闭运算填充小孔
    Mat kernel = getStructuringElement(MORPH_RECT, Size(16, 16));
    morphologyEx(hsvMask, hsvMask, MORPH_CLOSE, kernel);
    bitwise_not(hsvMask, hsvMask);
//    kernel=getStructuringElement(MORPH_RECT,Size(1,1));
//    morphologyEx(hsvMask,hsvMask,MORPH_OPEN,kernel);
    kernel = getStructuringElement(MORPH_RECT, Size(16, 16));
    morphologyEx(hsvMask, hsvMask, MORPH_CLOSE, kernel);
    //效果还行，有待提升
    return hsvMask;
}

Mat PipeProcessor::getPipeByThreshold(class cv::Mat image) {
    cvtColor(image, image, COLOR_RGB2GRAY);
//    Sobel(image,image,-1,1,1);
//    Mat imageBak=image.clone();
//    cvtColor(image,image,COLOR_RGB2GRAY);
    threshold(image, image, 10, 255, CV_THRESH_OTSU);
    //  定位效果不是非常理想，不如颜色空间的处理效果
    return image;
}

Mat PipeProcessor::getPipeByBoundary(class cv::Mat image) {
    /**
     * 通过边界来确定管道位置和走向
     * */
    cvtColor(image, image, COLOR_RGB2GRAY);
    Mat cannyImage;
    Canny(image, cannyImage, 30, 150);
    return cannyImage;


}

vector<Point> PipeProcessor::getContours(Mat image) {
    /**
     * 传入二值图像或者能够用来寻找边界的图像
     * */
    vector<vector<Point>> contours;
    findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    // 以下用来检测是否是相应的边界信息
    float maxSize = 100;
    vector<Point> maxContour;
    for (int i = 0; i < contours.size(); i++) {
        RotatedRect rect = minAreaRect(contours[i]);
        if (rect.size.height == 0 || rect.size.height == 0)
            continue;
        if (rect.size.height * rect.size.width > maxSize) {
            maxSize = rect.size.height * rect.size.width;
            maxContour = contours[i];
        }
    }
    return maxContour;
}


/**
 * 通过边界来寻找中心线
 * */
vector<Point> PipeProcessor::getMiddleLine(vector<Point> contour) {


    return vector<Point>();
}
