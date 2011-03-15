#include <cmath>
#include <vector>
#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "tools.h"

using namespace std;

void treatment(int value = 0);

IplImage* srcImg;
IplImage* cannyImg;
IplImage* houghImg;
IplImage* mergeImg;
IplImage* streetsImg;

/***** Hough *****/
int houghAccumulationThreshold = 30;
int houghMinLine = 30;
int houghMaxGap = 10;

/***** merge *****/
int mergeThreshold = 7;

/***** Streets *****/
int streetsTolerance10Deg = 10;
int streetsStreetWidth = 10;
int streetsDelta = 5;

int main(int argc, char** argv)
{
    if(argc == 2 && (srcImg = cvLoadImage(argv[1], 0)) != 0)
    {
        cannyImg = cvCreateImage(cvGetSize(srcImg), 8, 1);
        houghImg = cvCreateImage(cvGetSize(srcImg), 8, 3);
        mergeImg = cvCreateImage(cvGetSize(srcImg), 8, 3);

        cvNamedWindow("Source", CV_WINDOW_AUTOSIZE);

        cvNamedWindow("Hough", CV_WINDOW_AUTOSIZE);
        cvCreateTrackbar("Threshold:", "Hough", &houghAccumulationThreshold, 100, treatment);
        cvCreateTrackbar("Min length:", "Hough", &houghMinLine, 100, treatment);
        cvCreateTrackbar("Max gap:", "Hough", &houghMaxGap, 100, treatment);

        cvNamedWindow("Merge", CV_WINDOW_AUTOSIZE);
        cvCreateTrackbar("Threshold:", "Merge", &mergeThreshold, 100, treatment);
        cvCreateTrackbar("Angle (1/10 deg):", "Merge", &streetsTolerance10Deg, 50, treatment);

        treatment();
        cvWaitKey(0);

        cvReleaseImage(&srcImg);
        cvReleaseImage(&cannyImg);
        cvReleaseImage(&houghImg);
        cvReleaseImage(&mergeImg);
    }
}

void treatment(int)
{
    CvMemStorage* storage = cvCreateMemStorage(0);
    cvShowImage("Source", srcImg);

/******************************************************************************/
    CvSeq *cvLines = 0;

    cvCanny(srcImg, cannyImg, 50, 200);
    cvCvtColor(cannyImg, houghImg, CV_GRAY2BGR);

    cvLines = cvHoughLines2(cannyImg, storage, CV_HOUGH_PROBABILISTIC, 2, CV_PI / 180,
                            houghAccumulationThreshold, houghMinLine, houghMaxGap);
    vector<Segment> lines = convert(cvLines);

    draw(lines, houghImg);

    cvShowImage("Hough", houghImg);

/******************************************************************************/
    cvCvtColor(cannyImg, mergeImg, CV_GRAY2BGR);

    merge(lines, mergeThreshold, streetsTolerance10Deg * CV_PI / 1800);

    draw(lines, mergeImg);

    cvShowImage("Merge", mergeImg);

/******************************************************************************/
    cvReleaseMemStorage(&storage);
}
