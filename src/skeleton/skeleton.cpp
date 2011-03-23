#include <cv.h>
#include <highgui.h>
#include <iostream>

#include "Skeletonize.h"
#include "Intersections.h"

using namespace std;

enum InputType
{
    IMAGE,
    VIDEO,
    CAM,
    NONE
};

struct MouseParam
{
    bool clicked;
    CvPoint point;
};

MouseParam param;

void process(IplImage *colorSrcImg);
void onMouse(int event, int x, int y, int flags, void* param);

int main(int argc, char** argv)
{
    InputType input = NONE;
    if (argc == 3 && strcmp(argv[1], "-i") == 0)
    {
        input = IMAGE;
    }
    else if (argc == 3 && strcmp(argv[1], "-v") == 0)
    {
        input = VIDEO;
    }
    else if (argc == 2 && strcmp(argv[1], "-w") == 0)
    {
        input = CAM;
    }

    param.clicked = false;
    param.point = cvPoint(-1, -1);

    if (input != NONE)
    {
        cvNamedWindow("Source", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Binary", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Skeleton", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Intersections", CV_WINDOW_AUTOSIZE);

        IplImage *colorSrcImg = NULL;
        CvCapture *capture = NULL;

        if (input == IMAGE && ((colorSrcImg = cvLoadImage(argv[2])) != NULL))
        {
            cvShowImage("Source", colorSrcImg);
            cvSetMouseCallback("Source", onMouse, &param);
            while (!param.clicked) { cvWaitKey(100); }

            process(colorSrcImg);
            cvReleaseImage(&colorSrcImg);
            cvWaitKey();
        }
        else if(input == VIDEO && (capture = cvCaptureFromFile(argv[2])) != NULL)
        {
            IplImage *tmp = NULL;
            while ((tmp = cvQueryFrame(capture)) != NULL)
            {
                colorSrcImg = cvCloneImage(tmp);
                process(colorSrcImg);
                cvReleaseImage(&colorSrcImg);
                cvWaitKey(100);
            }
            cvReleaseCapture(&capture);
            cvWaitKey();
        }
        else if (input == CAM && ((capture = cvCaptureFromCAM(-1)) != NULL))
        {
            IplImage *tmp = NULL;
            while (true)
            {
                if ((tmp = cvQueryFrame(capture)) != NULL)
                {
                    colorSrcImg = cvCloneImage(tmp);
                    process(colorSrcImg);
                    cvReleaseImage(&colorSrcImg);
                    cvWaitKey(2);
                }
            }
            cvReleaseCapture(&capture);
            cvWaitKey();
        }

        cvDestroyWindow("Source");
        cvDestroyWindow("Binary");
        cvDestroyWindow("Skeleton");
        cvDestroyWindow("Intersections");
    }
}

void process(IplImage *colorSrcImg)
{
    IplImage *graySrcImg = cvCreateImage(cvGetSize(colorSrcImg), IPL_DEPTH_8U, 1);

    /***** Source *****/
    cvShowImage("Source", colorSrcImg);
    cvConvertImage(colorSrcImg, graySrcImg);

    /***** Threshold *****/
    if (param.point.x < 0 && param.point.x < 0)
    {
        cvThreshold(graySrcImg, graySrcImg, 240, 255, CV_THRESH_BINARY);
    }
    else
    {
        int color = cvGet2D(graySrcImg, param.point.y, param.point.x).val[0];
        int min = color - 10;
        if (min < 0) min = 0;
        int max = color + 10;
        if (max > 255) max = 255;

        cvThreshold(graySrcImg, graySrcImg, min, 255, CV_THRESH_TOZERO);
        cvThreshold(graySrcImg, graySrcImg, max, 255, CV_THRESH_TOZERO_INV);
        cvThreshold(graySrcImg, graySrcImg, 0, 255, CV_THRESH_BINARY);
    }
    cvMorphologyEx(graySrcImg, graySrcImg, NULL,
                   cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_RECT),
                   CV_MOP_CLOSE);
    cvShowImage("Binary", graySrcImg);

    /***** Skeleton *****/
    Skeletonize skel(graySrcImg);
    skel();
    cvShowImage("Skeleton", graySrcImg);

    /***** Intersections *****/
    vector<CvPoint> crossroads = Intersections(graySrcImg)();
    for (unsigned int i = 0; i < crossroads.size(); ++i)
    {
        cvCircle(colorSrcImg, crossroads[i], 2, CV_RGB(255, 0, 0), -1);
    }
    cvShowImage("Intersections", colorSrcImg);

    cvReleaseImage(&graySrcImg);
}

void onMouse(int event, int x, int y, int, void* param)
{
    if (event == CV_EVENT_LBUTTONUP)
    {
        MouseParam *mouseParam = reinterpret_cast<MouseParam*>(param);
        mouseParam->clicked = true;
        mouseParam->point = cvPoint(x, y);
    }
}
