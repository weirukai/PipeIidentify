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
     * 这个地方canny处理起来比较困难
     * 通过边界来确定管道位置和走向
     * */
    Mat imageGray;
    cvtColor(image, imageGray, CV_BGR2GRAY);
    Mat cannyImage;
    Canny(imageGray, cannyImage, 20, 60);
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
        if (rect.size.height == 0 || rect.size.width == 0)
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
vector<Point> PipeProcessor::getMiddleLine(vector<vector<Point>> contour) {
    /**
     * 传入三组边界信息，通过表决后确定效果最好的边界线，
     * 根据最佳边界线得到管道中线提供导航
     * */
    vector<Point> bestContour;


    for (int i = 0; i < bestContour.size(); ++i) {

    }


    return vector<Point>();
}

Mat PipeProcessor::locatePipe(Mat image) {
    return Mat();
}

Mat PipeProcessor::getObstruction(class cv::Mat image) {
    Mat hsvMask;
    Mat hsvMask2;
    cvtColor(image, hsvMask, COLOR_BGR2HSV);
    cvtColor(image, hsvMask2, COLOR_BGR2HSV);
//    split(image, hsvChannels);
    /**
     * 由于之前色块颜色得不同，具体颜色空间参数需要再进行调整
     * */
    /**
     * 橙色方块
     * */
    inRange(hsvMask, Scalar(9, 130, 35), Scalar(20, 255, 100), hsvMask);
    /**
     * 紫色的方块，紫色方块受环境影响较大，实际比赛中的黑色比较容易区分
     * */
    inRange(hsvMask2, Scalar(120, 90, 18), Scalar(145, 190, 40), hsvMask2);

    bitwise_or(hsvMask, hsvMask2, hsvMask);

    //形态学处理
    Mat kernel = getStructuringElement(MORPH_RECT, Size(16, 16));
    morphologyEx(hsvMask, hsvMask, MORPH_CLOSE, kernel);

    bitwise_not(hsvMask, hsvMask);
    return hsvMask;
}


int PipeProcessor::getObstructionType(Mat imageBinary, Mat origin) {
    //传入的应该是一个二值图像，筛选出了需要的部分

    isRect(imageBinary, origin);


    return -1;
}


bool PipeProcessor::isRect(Mat imageBinary, Mat origin) {
    vector<vector<Point>> contours;
    bitwise_not(imageBinary, imageBinary);//后景变成前景
    findContours(imageBinary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    //轮廓筛选
    float maxSize = 100;
    vector<Point> maxContour;
    for (int i = 0; i < contours.size(); i++) {
        RotatedRect rect = minAreaRect(contours[i]);
        if (rect.size.height == 0 || rect.size.width == 0)
            continue;
        if (rect.size.height * rect.size.width > maxSize) {
            maxSize = rect.size.height * rect.size.width;
            maxContour = contours[i];
        }
    }
    contours.clear();
    contours.push_back(maxContour);
    drawContours(origin, contours, -1, Scalar(0, 0, 255), 2);


    return false;
}

bool PipeProcessor::isOval(Mat image) {
    return false;
}
