//
// Created by 魏 on 2021/1/18.
//

#ifndef PIPEIDENTIFY_PIPEPROCESSOR_H
#define PIPEIDENTIFY_PIPEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
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
     * 通过表决器来判断位置
     * */
    static Mat getPipeByColor(Mat image);
    static Mat getPipeByThreshold(Mat image);
    static Mat getPipeByBoundary(Mat image);
    static Mat locatePipe(Mat image);


};


#endif //PIPEIDENTIFY_PIPEPROCESSOR_H
