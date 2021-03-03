//
// Created by 魏 on 2021/1/18.
//

#include "AdsorbateProcessor.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <cmath>
#include <numeric>

using namespace std;
using namespace cv;


float AdsorbateProcessor::getDistance(Point point1, Point point2, Point point_target) {

    float temp1 = (point2.y - point1.y) * point_target.x + (point1.x - point2.x) * point_target.y +
                  (point1.y * point2.x - point2.y * point1.x);
    float temp2 = sqrt(pow(point2.y - point1.y, 2) + pow(point1.x - point2.x, 2));
    return abs(temp1) / temp2;
}

float AdsorbateProcessor::getMinDis(vector<Point> contour_poly, Point point) {
    float minDis = FLT_MAX;
    float tempDis = -1;
    for (int i = 0; i < contour_poly.size() - 1; ++i) {
        if ((tempDis = getDistance(contour_poly[i], contour_poly[i + 1], point)) < minDis)
            minDis = tempDis;
    }
    if ((tempDis = getDistance(contour_poly[contour_poly.size() - 1], contour_poly[0], point)) < minDis)
        minDis = tempDis;

    return minDis;
}


double AdsorbateProcessor::culVariance(vector<double> distances) {

    double sum = std::accumulate(std::begin(distances), std::end(distances), 0.0);
    double mean = sum / distances.size(); //均值

    double accum = 0.0;
    std::for_each(std::begin(distances), std::end(distances), [&](const double d) {
        accum += (d - mean) * (d - mean);
    });

    double variance = sqrt(accum / (distances.size() - 1));
    return variance;
}


int AdsorbateProcessor::judgeByVariance(vector<cv::Point> contour_poly, vector<cv::Point> contour, Mat origin) {
    /***///通过调整lineFitLen和threshold来对精确度进行调整！！！
    int len = contour.size();
    int lineFitLen = len / 7;
    bool *isLineFlag = new bool[contour.size()];//用来记录直线点位
    for (int k = 0; k < contour.size(); ++k) {
        isLineFlag[k] = false;
    }
    double variance = -1.0;
    double threshold = 3.0;
    vector<double> disContainer;
    for (int i = 0; i < contour.size() && i + lineFitLen < contour.size(); i += 10, variance = -1.0) {


        /***如下只是绘制出i-i+lineLen一段的直线***/
        line(origin, contour[i], contour[i + lineFitLen], Scalar(255, 0, 255), 2, 8);
        namedWindow("windowName", 0);
        resizeWindow("windowName", 640, 512);
        imshow("windowName", origin);
        //测试视频或者图片的时候切换
        waitKey(100);




        disContainer.clear();
        for (int j = i; j < i + lineFitLen; ++j) {
            disContainer.push_back(getDistance(contour[i], contour[i + lineFitLen], contour[j]));
        }
        variance = culVariance(disContainer);
        if (variance < threshold) {
            //认为探测到的是直线，并做删除处理
            for (int j = i; j < i + lineFitLen; ++j) {
                isLineFlag[j] = true;
            }
        } else if (variance >= threshold) {
            //不做处理


        }


    }
    //拿到所有被标记为直线的点的述目同边界长度进行比较，进而判断是否是立方体
    int linePointCount = 0;
    for (int l = 0; l < contour.size(); ++l) {
        if (isLineFlag[l]) linePointCount++;
    }


    double ratio = double(linePointCount) / double(contour.size());
    cout<<ratio<<endl;

    if (ratio > 0.6) return RECT;
    else return OVAL;


}


int AdsorbateProcessor::judgeByDis(vector<Point> contour_poly, vector<Point> contour) {

    //通过逼近包围盒和边界的顶点的距离和区分立方体和圆柱体
    float disSum = 0;
    float judgeFlag = 0;
    for (int i = 0; i < contour.size(); ++i) {
        disSum += getMinDis(contour_poly, contour[i]);
    }
    judgeFlag = disSum / contour.size();
    if (judgeFlag < 0) {
        return -1;
    }
}

int AdsorbateProcessor::judgeByAreaRatio(vector<Point> contour_poly, vector<Point> contour) {

    double polyArea = contourArea(contour_poly);
    double realArea = contourArea(contour);
    double ratio = realArea / polyArea;

    cout << ratio << endl;
    if (ratio > 1.03) return RECT;
    return OVAL;
}


/**  圆形度判别**/
int AdsorbateProcessor::judgeByRoundness(vector<cv::Point> contour_poly, vector<cv::Point> contour) {
    double area = contourArea(contour);
    double length = arcLength(contour, true);
    double roundness = (4 * CV_PI * area) / (length * length);
    cout << roundness << endl;

    return RECT;
}

int AdsorbateProcessor::judgeByLineNum(vector<cv::Point> contour_poly, vector<cv::Point> contour) {
//    int len = contour_poly.size();
    double len_poly = arcLength(contour_poly, true);
    double len_contour = arcLength(contour, true);
    cout << len_contour / len_poly << endl;
    if (len_contour / len_poly < 10) return RECT;
    return OVAL;
}

/**
 * 障碍物位置确定
 * */
Mat AdsorbateProcessor::getObstruction(class cv::Mat image) {
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
    inRange(hsvMask2, Scalar(120, 90, 9), Scalar(145, 190, 40), hsvMask2);

    bitwise_or(hsvMask, hsvMask2, hsvMask);

    //形态学处理
    Mat kernel = getStructuringElement(MORPH_RECT, Size(20, 20));
    morphologyEx(hsvMask, hsvMask, MORPH_CLOSE, kernel);

    kernel = getStructuringElement(MORPH_RECT, Size(50, 50));
    morphologyEx(hsvMask, hsvMask, MORPH_OPEN, kernel);

    bitwise_not(hsvMask, hsvMask);
    return hsvMask;
}


int AdsorbateProcessor::getObstructionType(Mat imageBinary, Mat origin) {
    //传入的应该是一个二值图像，筛选出了需要的部分

    bool flag = isRect(imageBinary, origin);


    return -1;
}


bool AdsorbateProcessor::isRect(Mat imageBinary, Mat origin) {
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
    /**图像中没有障碍物时候的处理*/
    if (maxContour.empty())
        return false;
    vector<Point> contours_poly(contours.size());


//    double acreage=contourArea(contours_poly);
//    double circumference=arcLength(contours_poly, true);

    approxPolyDP(Mat(maxContour), contours_poly, 20, true);
//    double m=acreage*circumference;
//    double n=acreage/circumference;

    //绘制邻近轮廓
//    contours.clear();
//    contours.push_back(contours_poly);
//    drawContours(origin, contours, -1, Scalar(0, 0, 255), 2);
    //绘制轮廓
    contours.clear();
    contours.push_back(maxContour);
    drawContours(origin, contours, -1, Scalar(0, 0, 255), 2);





    ///绘制外接矩形
    vector<Point2f> boxPts(4);
    RotatedRect rect = minAreaRect(contours_poly);
    rect.points(boxPts.data());
    int type = judgeByVariance(contours_poly, maxContour,origin);
//    if (type==RECT) {
//        for (int j = 0; j < 4; j++) {
//            line(origin, boxPts[j], boxPts[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);  //绘制最小外接矩形每条边
//        }
//        putText(origin, "cube", Point(50, 60), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 23, 0), 4, 8);
//    } else if(type==OVAL) {
//        for (int j = 0; j < 4; j++) {
//            line(origin, boxPts[j], boxPts[(j + 1) % 4], Scalar(255, 0, 255), 2, 8);  //绘制最小外接矩形每条边
//        }
//        putText(origin, "Cylinder", Point(50, 60), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 23, 0), 4, 8);
//    }

//    judgeByDis(contours_poly, maxContour);


    return type;
}

int AdsorbateProcessor::judgeByAngle(vector<Point> contour_poly) {
    vector<int> results;
    int temp1 = 0, temp2 = 0;
    int count1 = 0, count2 = 0;
    Mat data_pts = Mat(contour_poly.size(), 2, CV_64FC1); //使用mat来保存数据，也是为了后面pca处理需要
    for (int i = 0; i < data_pts.rows; ++i) {
        data_pts.at<double>(i, 0) = contour_poly[i].x;
        data_pts.at<double>(i, 1) = contour_poly[i].y;
    }
    //执行PCA分析
    PCA pca_analysis(data_pts, Mat(), CV_PCA_DATA_AS_ROW);
    //获得最主要分量，在本例中，对应的就是轮廓中点，也是图像中点
    Point pos = Point(pca_analysis.mean.at<double>(0, 0), pca_analysis.mean.at<double>(0, 1));

    for (int j = 0; j < contour_poly.size() - 2; j++) {
        temp1 = AdsorbateProcessor::getAngle(contour_poly[j], contour_poly[j + 1], pos);
        temp2 = AdsorbateProcessor::getAngle(contour_poly[j + 1], contour_poly[j + 2], pos);
        results.push_back(temp1 + temp2);
    }
    temp1 = AdsorbateProcessor::getAngle(contour_poly[contour_poly.size() - 3], contour_poly[contour_poly.size() - 2],
                                         pos);
    temp2 = AdsorbateProcessor::getAngle(contour_poly[contour_poly.size() - 2], contour_poly[contour_poly.size() - 1],
                                         pos);
    results.push_back(temp1 + temp2);
    temp1 = AdsorbateProcessor::getAngle(contour_poly[contour_poly.size() - 2], contour_poly[contour_poly.size() - 1],
                                         pos);
    temp2 = AdsorbateProcessor::getAngle(contour_poly[contour_poly.size() - 1], contour_poly[0], pos);
    results.push_back(temp1 + temp2);
    for (int k = 0; k < results.size(); k++) {
        //if(results[k]>180) return RECT;
        if (results[k] > 100 && results[k] < 170) count1++;
        else count2++;
    }
    if (count1 > 1.2 * count2) return OVAL;
    else return RECT;
}

int AdsorbateProcessor::getAngle(Point apex1, Point apex2, Point pos) {
    int temp1 = 0, temp2 = 0, temp = 0;
    temp1 = (pos.x - apex2.x) * (apex1.x - apex2.x) + (pos.y - apex2.y) * (apex1.y - apex2.y);
    temp2 = sqrt((pos.x - apex2.x) * (pos.x - apex2.x) + (pos.y - apex2.y) * (pos.y - apex2.y)) *
            sqrt((apex1.x - apex2.x) * (apex1.x - apex2.x) + (apex1.y - apex2.y) * (apex1.y - apex2.y));
    temp = acos(float(temp1) / float(temp2)) * 180 / CV_PI;
    return temp;
}

bool AdsorbateProcessor::isOval(Mat image) {
    return false;
}