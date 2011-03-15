#include <cv.h>
#include <highgui.h>
#include <iostream>

#include "Skeletonize.h"
#include "Intersections.h"

using namespace std;

int main(int argc, char** argv)
{
    IplImage *colorSrcImg = NULL;

int64 start = cvGetTickCount();
    if (argc == 2 && (colorSrcImg = cvLoadImage(argv[1])) != NULL)
    {
        IplImage *graySrcImg = cvCreateImage(cvGetSize(colorSrcImg), IPL_DEPTH_8U, 1);
        cvNamedWindow("Source", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Binary", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Skeleton", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Intersections", CV_WINDOW_AUTOSIZE);

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
cout << (cvGetTickCount() - start) / (1000.f * cvGetTickFrequency()) << " ms" << endl;
        for (unsigned int i = 0; i < crossroads.size(); ++i)
        {
            cvCircle(colorSrcImg, crossroads[i], 2, CV_RGB(255, 0, 0), -1);
        }
        cvShowImage("Intersections", colorSrcImg);

        cvWaitKey();

        cvReleaseImage(&graySrcImg);
        cvReleaseImage(&colorSrcImg);
        cvDestroyWindow("Source");
        cvDestroyWindow("Binary");
        cvDestroyWindow("Skeleton");
        cvDestroyWindow("Intersections");
    }
}
