//
// Created by 魏 on 2021/1/18.
//

#ifndef PIPEIDENTIFY_VIDEOPROCESSOR_H
#define PIPEIDENTIFY_VIDEOPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
using namespace  cv;
class VideoProcessor {
    /**
     *
     *  多用于静态处理，多为静态方法
     * */

public:
    static Mat getVideoImage(String videoPath);




};


#endif //PIPEIDENTIFY_VIDEOPROCESSOR_H
