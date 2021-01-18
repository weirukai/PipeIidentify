//
// Created by 魏 on 2021/1/18.
//

#ifndef PIPEIDENTIFY_GENERALPROCESSOR_H
#define PIPEIDENTIFY_GENERALPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

/**
 * 公共处理区域，静态为主
 *
 * */
class GeneralProcessor {


public:
    static cv::Mat preProcess(cv::Mat image);
    static cv::Mat showImage(cv::String windowName,cv::Mat image );

};


#endif //PIPEIDENTIFY_GENERALPROCESSOR_H
