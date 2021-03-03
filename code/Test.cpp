//
// Created by κ on 2021/1/18.
//

#include "Test.h"
#include "PipeProcessor.h"
#include "GeneralProcessor.h"
#include "AdsorbateProcessor.h"
#include <io.h>

const cv::String Test::imagePath = "D:\\ClCodes\\PipeIdentify\\images\\1000.jpg";
//const cv::String Test::videoPath = "D:\\ClCodes\\PipeIdentify\\videos\\21-5-38.avi";
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
            PipeProcessor::getMiddleLine(contour, imageBak);

            //绘制外接矩形
//            vector<Point2f> boxPts(4);
//            RotatedRect rect = minAreaRect(contour);
//            rect.points(boxPts.data());
//            for (int j = 0; j < 4; j++) {
//                line(imageBak, boxPts[j], boxPts[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);  //绘制最小外接矩形每条边
//            }


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
    image = GeneralProcessor::preProcess(image);
    image = PipeProcessor::getPipeByColor(image);
    vector<Point> contour = PipeProcessor::getContours(image);
    //绘制边界
    vector<vector<Point>> contours;
    contours.push_back(contour);
    drawContours(imageBak, contours, -1, Scalar(0, 0, 255), 2);
    PipeProcessor::getMiddleLine(contour, imageBak);

    //绘制外接矩形
//            vector<Point2f> boxPts(4);
//            RotatedRect rect = minAreaRect(contour);
//            rect.points(boxPts.data());
//            for (int j = 0; j < 4; j++) {
//                line(imageBak, boxPts[j], boxPts[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);  //绘制最小外接矩形每条边
//            }


    imageBak.copyTo(image, image);
    showImage("video", imageBak);


}

void Test::test4() {
    Mat image = imread(Test::imagePath, IMREAD_ANYCOLOR);
    Mat imageBak = image.clone();
    Mat obstructionImage = AdsorbateProcessor::getObstruction(imageBak);
    showImage("obstruction", obstructionImage);
    AdsorbateProcessor::isRect(obstructionImage, image);
    showImage("origin", image);
}

void Test::test5() {
    VideoCapture capture(videoPath);
    Mat image;
    Mat imageMask;
    Mat imageBak;
    while (true) {
        if (capture.read(image) && capture.isOpened()) {
            imageBak = image.clone();
            image = GeneralProcessor::preProcess(image);
            imageMask = AdsorbateProcessor::getObstruction(imageBak);
            int type = AdsorbateProcessor::getObstructionType(imageMask, imageBak);

            //绘制外接矩形
//            vector<Point2f> boxPts(4);
//            RotatedRect rect = minAreaRect(contour);
//            rect.points(boxPts.data());
//            for (int j = 0; j < 4; j++) {
//                line(imageBak, boxPts[j], boxPts[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);  //绘制最小外接矩形每条边
//            }


            imageBak.copyTo(image, image);
            showImage("video", imageBak);

        } else {
            break;
        }
    }

}

/**
 * 数据统计时候使用
 * */
void Test::test6() {
    char *filePath = "D:\\ClCodes\\PipeIdentify\\images\\cube";
    vector<string> files;
    getFiles(filePath, files);
    int cubeCount = 0;
    int CylinderCount = 0;
    int type = -1;
    for (int i = 0; type = -1, i < files.size(); ++i) {
        Mat image = imread(files[i], IMREAD_ANYCOLOR);
        Mat imageBak = image.clone();
        Mat obstructionImage = AdsorbateProcessor::getObstruction(imageBak);
        //showImage("obstruction",obstructionImage);
        type = AdsorbateProcessor::isRect(obstructionImage, image);
        if (type == RECT)
            cubeCount++;
        else if (type == OVAL)
            CylinderCount++;
    }


    return;
}





/**
 * 工具函数
 * **/

void Test::showImage(cv::String windowName, cv::Mat image) {

    namedWindow(windowName, 0);

    resizeWindow(windowName, 640, 512);
    imshow(windowName, image);
    //测试视频或者图片的时候切换
    waitKey(0);
//    waitKey(0);
//    destroyAllWindows();
}

void Test::getFiles(string path, vector<string> &files) {
    //文件句柄
    long hFile = 0;
    //文件信息
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
        do {
            //如果是目录,迭代之
            //如果不是,加入列表
            if ((fileinfo.attrib & _A_SUBDIR)) {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            } else {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}