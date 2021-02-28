//
// Created by κ on 2021/1/18.
//

#include "Test.h"
#include "PipeProcessor.h"
#include "GeneralProcessor.h"

const cv::String Test::imagePath = "D:\\ClCodes\\PipeIdentify\\images\\430.jpg";
const cv::String Test::videoPath = "D:\\ClCodes\\PipeIdentify\\videos\\20-52-50.avi";

/**
 * 为保证main函数的结构清晰，测试需要在这里进行
 * 除main外，其他文件中不应该调用test测试类
 * */
void Test::test() {
    /*******
     *
     * 测试处，请在此处进行相应的测试
     *
     * ******/
    Mat image = imread(Test::imagePath, IMREAD_ANYCOLOR);
    Mat imageBak = image.clone();
    Mat imageGray;
//    image = GeneralProcessor::preProcess(image);
    imageGray = PipeProcessor::getPipeByBoundary(image);
//    vector<Point> contour = PipeProcessor::getContours(image);
//    vector<vector<Point>> contours;
//    contours.push_back(contour);
//    drawContours(imageBak, contours, -1, Scalar(0, 255, 255), 2);
    imageBak.copyTo(image, image);
    showImage("threshold", imageGray);
    waitKey(0);
}

/**视频实体测试*/
void Test::test2() {
    VideoCapture capture(videoPath);
    Mat image;
    while (true) {
        if (capture.read(image) && capture.isOpened()) {
            Mat imageBak = image.clone();
            image = GeneralProcessor::preProcess(image);
            image = PipeProcessor::getPipeByColor(image);
            vector<Point> contour = PipeProcessor::getContours(image);
            if (contour.empty())
                continue;
            //绘制边界
            vector<vector<Point>> contours;
            contours.push_back(contour);
            drawContours(imageBak, contours, -1, Scalar(0, 0, 255), 2);

            //绘制外接矩形
            vector<Point2f> boxPts(4);
            RotatedRect rect = minAreaRect(contour);
            rect.points(boxPts.data());
            for (int j = 0; j < 4; j++) {
                line(imageBak, boxPts[j], boxPts[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);  //绘制最小外接矩形每条边
            }


            imageBak.copyTo(image, image);
            showImage("video", imageBak);
//            imshow("video",image);
//            waitKey(1);
        } else {
            break;
        }
    }
}


void Test::test3() {
    Mat image = imread(Test::imagePath, IMREAD_ANYCOLOR);
    Mat imageBak = image.clone();
    Mat cannyImage = PipeProcessor::getPipeByBoundary(image);


}

void Test::test4(){
    Mat image = imread(Test::imagePath, IMREAD_ANYCOLOR);
    Mat imageBak = image.clone();
    Mat obstructionImage = PipeProcessor::getObstruction(imageBak);
    showImage("obstruction",obstructionImage);
    PipeProcessor::isRect(obstructionImage,image);
    showImage("origin",image);
}







void Test::showImage(cv::String windowName, cv::Mat image) {

    namedWindow(windowName, 0);

    resizeWindow(windowName, 640, 512);
    imshow(windowName, image);
    //测试视频或者图片的时候切换
    waitKey(0);
//    waitKey(0);
//    destroyAllWindows();
}