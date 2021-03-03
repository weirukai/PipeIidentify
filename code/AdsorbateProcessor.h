//
// Created by 魏 on 2021/1/18.
//

#ifndef PIPEIDENTIFY_ADSORBATEPROCESSOR_H
#define PIPEIDENTIFY_ADSORBATEPROCESSOR_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

#define RECT 0
#define OVAL 1
using namespace std;
using namespace cv;

class AdsorbateProcessor {
public:
    static float getMinDis(vector<Point> contour_poly, Point point);

    static int judgeByDis(vector<Point> contour_poly, vector<Point> contour);

    /***
     * 对障碍物进行定位
     * */
    static Mat getObstruction(Mat image);


    /**
     * 传入三个点，得到目标到到其余两点构成的直线的距离
     * */
    static float getDistance(Point point1, Point point2, Point point_target);

    static bool isRect(Mat imageBinary, Mat origin);

    static int getObstructionType(Mat imageBinary, Mat origin);


    static int judgeByAngle(vector<Point> contour_poly);

    static int getAngle(Point apex1, Point apex2, Point pos);

    static bool isOval(Mat image);

    static int judgeByAreaRatio(vector<Point> contour_poly, vector<Point> contour);

    static int judgeByRoundness(vector<cv::Point> contour_poly, vector<cv::Point> contour);

    static int judgeByLineNum(vector<cv::Point> contour_poly, vector<cv::Point> contour);

    static double culVariance(vector<double> distances);

    static int judgeByVariance(vector<cv::Point> contour_poly, vector<cv::Point> contour);
    static int judgeByVariance(vector<cv::Point> contour_poly, vector<cv::Point> contour,Mat origin);
};


#endif //PIPEIDENTIFY_ADSORBATEPROCESSOR_H
