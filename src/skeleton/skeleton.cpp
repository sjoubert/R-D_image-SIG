#include <cv.h>
#include <highgui.h>
#include <iostream>

#include "Skeletonize.h"
#include "Intersections.h"

using namespace std;

void process(IplImage *colorSrcImg);

int main(int argc, char** argv)
{
    if (argc == 3)
    {
        cvNamedWindow("Source", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Binary", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Skeleton", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Intersections", CV_WINDOW_AUTOSIZE);
        
        IplImage *colorSrcImg = NULL;
        CvCapture *capture = NULL;
        if (strcmp(argv[1], "-i") == 0 &&
            (colorSrcImg = cvLoadImage(argv[2])) != NULL)
        {
            process(colorSrcImg);   
            cvReleaseImage(&colorSrcImg);
            cvWaitKey();
        }
        else if(strcmp(argv[1], "-v") == 0 &&
                (capture = cvCaptureFromFile(argv[2])) != NULL)
        {
            IplImage *tmp = NULL;
            while ((tmp = cvQueryFrame(capture)) != NULL)
            {
                colorSrcImg = cvCloneImage(tmp);
                process(colorSrcImg);
                cvReleaseImage(&colorSrcImg);
                cvWaitKey(2);
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
    cvThreshold(graySrcImg, graySrcImg, 240, 255, CV_THRESH_BINARY);
    cvMorphologyEx(graySrcImg, graySrcImg, NULL,
                   cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT),
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
