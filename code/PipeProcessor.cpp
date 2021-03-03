//
// Created by κ on 2021/1/18.
//

#include "PipeProcessor.h"
#include <vector>
#include <algorithm>
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
vector<Point> PipeProcessor::getMiddleLine(vector<Point> contour,Mat image) {
    /**
     * 传入三组边界信息，通过表决后确定效果最好的边界线，
     * 根据最佳边界线得到管道中线提供导航
     * */
    vector<Point> bestContour;
    bestContour=contour;
    Mat data_pts  = Mat(bestContour.size(),  2, CV_64FC1); //使用mat来保存数据，也是为了后面pca处理需要
    for ( int i  =  0; i  < data_pts.rows;  ++i)
    {
        data_pts.at < double >(i,  0)  = bestContour[i].x;
        data_pts.at < double >(i,  1)  = bestContour[i].y;
    }
    //执行PCA分析
    PCA pca_analysis(data_pts, Mat(), CV_PCA_DATA_AS_ROW);
    //获得最主要分量，得到轮廓的中点
    Point pos  = Point(pca_analysis.mean.at < double >( 0,  0),pca_analysis.mean.at < double >( 0,  1));

    vector<Point> tempPoints;
    vector<Point> middlePoints;
    for (int j = image.rows; j >image.rows/2 ;j=j-1) {
        tempPoints.clear();
        for (int i = 0; i < bestContour.size(); ++i) {
            if (bestContour[i].y==j){
                tempPoints.push_back(bestContour[i]);
            }
        }
        if (tempPoints.size()<2)
            continue;
        //找这些点中横坐标距离最大的点
        Point middle;
        Point min;
        min.x=image.cols;
        Point max;
        max.x=0;

        for (int k = 0; k < tempPoints.size(); ++k) {
            if (tempPoints[k].x<min.x)
                min=tempPoints[k];
        }
        for (int k = 0; k < tempPoints.size(); ++k) {
            if (tempPoints[k].x>max.x)
                max=tempPoints[k];
        }

        if (max.x-min.x>100)
        {
            middle.x=(max.x+min.x)/2;
            middle.y=(max.y);
        }
        middlePoints.push_back(middle);
    }
    if (middlePoints.size()>0)
        //其它中点备用用于提升精确度
    line(image,pos,middlePoints[0],Scalar(0, 0, 255),2);


    vector<Point> result;
    result.push_back(pos);
    result.push_back(middlePoints[0]);
    return result;
}

Mat PipeProcessor::locatePipe(Mat image) {
    return Mat();
}
//
//Mat PipeProcessor::getObstruction(class cv::Mat image) {
//    Mat hsvMask;
//    Mat hsvMask2;
//    cvtColor(image, hsvMask, COLOR_BGR2HSV);
//    cvtColor(image, hsvMask2, COLOR_BGR2HSV);
////    split(image, hsvChannels);
//    /**
//     * 由于之前色块颜色得不同，具体颜色空间参数需要再进行调整
//     * */
//    /**
//     * 橙色方块
//     * */
//    inRange(hsvMask, Scalar(9, 130, 35), Scalar(20, 255, 100), hsvMask);
//    /**
//     * 紫色的方块，紫色方块受环境影响较大，实际比赛中的黑色比较容易区分
//     * */
//    inRange(hsvMask2, Scalar(120, 90, 9), Scalar(145, 190, 40), hsvMask2);
//
//    bitwise_or(hsvMask, hsvMask2, hsvMask);
//
//    //形态学处理
//    Mat kernel = getStructuringElement(MORPH_RECT, Size(20, 20));
//    morphologyEx(hsvMask, hsvMask, MORPH_CLOSE, kernel);
//
//    kernel = getStructuringElement(MORPH_RECT, Size(50, 50));
//    morphologyEx(hsvMask, hsvMask, MORPH_OPEN, kernel);
//
//    bitwise_not(hsvMask, hsvMask);
//    return hsvMask;
//}
//
//
//int PipeProcessor::getObstructionType(Mat imageBinary, Mat origin) {
//    //传入的应该是一个二值图像，筛选出了需要的部分
//
//    bool flag=isRect(imageBinary, origin);
//
//
//
//    return -1;
//}
//
//
//bool PipeProcessor::isRect(Mat imageBinary, Mat origin) {
//    vector<vector<Point>> contours;
//    bitwise_not(imageBinary, imageBinary);//后景变成前景
//    findContours(imageBinary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//    //轮廓筛选
//    float maxSize = 100;
//    vector<Point> maxContour;
//    for (int i = 0; i < contours.size(); i++) {
//        RotatedRect rect = minAreaRect(contours[i]);
//        if (rect.size.height == 0 || rect.size.width == 0)
//            continue;
//        if (rect.size.height * rect.size.width > maxSize) {
//            maxSize = rect.size.height * rect.size.width;
//            maxContour = contours[i];
//        }
//    }
//    /**图像中没有障碍物时候的处理*/
//    if(maxContour.empty())
//        return false;
//    vector<Point> contours_poly(contours.size());
//
//
////    double acreage=contourArea(contours_poly);
////    double circumference=arcLength(contours_poly, true);
//
//    approxPolyDP(Mat(maxContour), contours_poly, 20, true);
////    double m=acreage*circumference;
////    double n=acreage/circumference;
//
//    //绘制邻近轮廓
//    contours.clear();
//    contours.push_back(contours_poly);
//    drawContours(origin, contours, -1, Scalar(0, 0, 255), 2);
//
//
//
//
//    int len=contours_poly.size();
//
//
//    ///绘制外接矩形
//    vector<Point2f> boxPts(4);
//    RotatedRect rect = minAreaRect(contours_poly);
//    rect.points(boxPts.data());
//    if (len<10)
//    {
//        for (int j = 0; j < 4; j++) {
//            line(origin, boxPts[j], boxPts[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);  //绘制最小外接矩形每条边
//        }
//        putText(origin,"cube",Point(50,60),FONT_HERSHEY_SIMPLEX,1,Scalar(255,23,0),4,8);
//    } else{
//        for (int j = 0; j < 4; j++) {
//            line(origin, boxPts[j], boxPts[(j + 1) % 4], Scalar(255, 0, 255), 2, 8);  //绘制最小外接矩形每条边
//        }
//        putText(origin,"Cylinder",Point(50,60),FONT_HERSHEY_SIMPLEX,1,Scalar(255,23,0),4,8);
//    }
//
//    judgeByAngle(contours_poly);
//
//    return len < 10;
//}
//
//int PipeProcessor::judgeByAngle(vector<Point> contour_poly)
//{
//    vector<int> results;
//    int temp1,temp2;
//    int count1,count2;
//    Mat data_pts  = Mat(contour_poly.size(),  2, CV_64FC1); //使用mat来保存数据，也是为了后面pca处理需要
//    for ( int i  =  0; i  < data_pts.rows;  ++i)
//    {
//        data_pts.at < double >(i,  0)  = contour_poly[i].x;
//        data_pts.at < double >(i,  1)  = contour_poly[i].y;
//    }
//    //执行PCA分析
//    PCA pca_analysis(data_pts, Mat(), CV_PCA_DATA_AS_ROW);
//    //获得最主要分量，在本例中，对应的就是轮廓中点，也是图像中点
//    Point pos  = Point(pca_analysis.mean.at < double >( 0,  0),pca_analysis.mean.at < double >( 0,  1));
//
//    for (int j = 0; j < contour_poly.size()-2; j++) {
//        temp1=PipeProcessor::getAngle(contour_poly[j],contour_poly[j+1],pos);
//        temp2=PipeProcessor::getAngle(contour_poly[j+1],contour_poly[j+2],pos);
//        results.push_back(temp1+temp2);
//    }
//    temp1=PipeProcessor::getAngle(contour_poly[contour_poly.size()-3],contour_poly[contour_poly.size()-2],pos);
//    temp2=PipeProcessor::getAngle(contour_poly[contour_poly.size()-2],contour_poly[contour_poly.size()-1],pos);
//    results.push_back(temp1+temp2);
//    temp1=PipeProcessor::getAngle(contour_poly[contour_poly.size()-2],contour_poly[contour_poly.size()-1],pos);
//    temp2=PipeProcessor::getAngle(contour_poly[contour_poly.size()-1],contour_poly[0],pos);
//    results.push_back(temp1+temp2);
//    for (int k = 0; k < results.size(); k++) {
//        if(results[k]>100)  count1++;
//        else count2++;
//    }
//    if(count1>1.2*count2)  return OVAL;
//    else return RECT;
//}
//
//int PipeProcessor::getAngle(Point apex1, Point apex2, Point pos) {
//    int temp1,temp2,temp;
//    temp1=(pos.x-apex2.x)*(apex1.x-apex2.x)+(pos.y-apex2.y)*(apex1.y-apex2.y);
//    temp2=sqrt(((pos.x-apex2.x)^2+(pos.y-apex2.y)^2)*((apex1.x-apex2.x)^2+(apex1.y-apex2.y)^2));
//    temp=acos(float(temp1)/float(temp2));
//    return temp;
//}
//
//bool PipeProcessor::isOval(Mat image) {
//    return false;
//}
