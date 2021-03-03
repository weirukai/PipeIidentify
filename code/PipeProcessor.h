//
// Created by 魏 on 2021/1/18.
//

#ifndef PIPEIDENTIFY_PIPEPROCESSOR_H
#define PIPEIDENTIFY_PIPEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <vector>
#define RECT 0
#define OVAL 1
using namespace std;
using namespace cv;

class PipeProcessor {
private:
public:
    /**
     * 1.颜色
     *
     * 2.边缘
     * 3.阈值
     *
     * 可以通过表决器来判断位置
     * */
    static Mat getPipeByColor(Mat image);

    static Mat getPipeByThreshold(Mat image);

    static Mat getPipeByBoundary(Mat image);

    static Mat locatePipe(Mat image);

    /**获取二值图像的边缘，后面利用边缘探测前进方向*/
    static vector<Point> getContours(Mat image);

    /**通过边界寻找中心线，根据中心线进行导航*/

    static vector<Point> getMiddleLine(vector<Point> contour,Mat image);

//    static Mat getObstruction(Mat image);
//
//    static int getObstructionType(Mat image);
//
//    static bool isRect(Mat image);
//
//    static bool isOval(Mat image);
//
//
//    static bool isRect(Mat imageBinary, Mat origin);
//
//    static int getObstructionType(Mat imageBinary, Mat origin);
//
//
//
//    static int judgeByAngle(vector<Point> contour_poly);
//
//    static int getAngle(Point apex1, Point apex2, Point pos);
};


#endif //PIPEIDENTIFY_PIPEPROCESSOR_H
