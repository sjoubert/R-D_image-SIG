#include "tools.h"

using namespace std;

vector<Segment> convert(CvSeq *lines)
{
    vector<Segment> seg;

    for (int i = 0; i < lines->total; ++i)
    {
        CvPoint *p = (CvPoint*)cvGetSeqElem(lines, i);
        seg.push_back(Segment(Point(p[0].x, p[0].y), Point(p[1].x, p[1].y)));
    }

    return seg;
}

void draw(const vector<Segment> &lines, IplImage *img)
{
    for(unsigned int i = 0; i < lines.size(); ++i)
    {
        Segment line = lines[i];
        CvPoint p1 = cvPoint(line.p1().x, line.p1().y);
        CvPoint p2 = cvPoint(line.p2().x, line.p2().y);
        cvLine(img, p1, p2, CV_RGB(255,0,0), 1, 4);
    }
}

void merge(vector<Segment> &lines, int threshold, double toleranceRad)
{
    bool modif = true;
    while (modif)
    {
        modif = false;

        for (unsigned int i = 0; i < lines.size(); ++i)
        {
            Segment p1 = lines[i];

            for (unsigned int j = i + 1; j < lines.size(); ++j)
            {
                Segment p2 = lines[j];

                if (abs(p1.theta() - p2.theta()) < toleranceRad &&
                    areClose(p1, p2, threshold))
                {
                    p1 = lines[i] = doMerge(p1, p2);
                    lines.erase(lines.begin() + j);
                    --j;
                    modif = true;
                }
            }
        }
    }
}

Segment doMerge(const Segment &tgt, const Segment &src)
{
    int xMin = min(min(tgt.p1().x, tgt.p2().x), min(src.p1().x, src.p2().x));
    int xMax = max(max(tgt.p1().x, tgt.p2().x), max(src.p1().x, src.p2().x));
    int yMin = min(min(tgt.p1().y, tgt.p2().y), min(src.p1().y, src.p2().y));
    int yMax = max(max(tgt.p1().y, tgt.p2().y), max(src.p1().y, src.p2().y));

    Point topLeft(xMin, yMax);
    Point topRight(xMax, yMax);
    Point bottomLeft(xMin, yMin);
    Point bottomRight(xMax, yMin);

    if (tgt.theta() < CV_PI / 2)
    {
        return Segment(topLeft, bottomRight);
    }
    else
    {
        return Segment(topRight, bottomLeft);
    }
}

bool areClose(const Segment &s1, const Segment &s2, int threshold)
{
    return s1.p1().distanceToSegment(s2) < threshold ||
           s1.p2().distanceToSegment(s2) < threshold ||
           s2.p1().distanceToSegment(s1) < threshold ||
           s2.p2().distanceToSegment(s1) < threshold;
}
