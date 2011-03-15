#ifndef SJ_POINT_H_05122010221410
#define SJ_POINT_H_05122010221410

#include <cmath>
#include <iostream>

#include <cv.h>

class Segment;

struct Point
{
    int x;
    int y;

    inline Point(int x = 0, int y = 0)
    {
        this->x = x;
        this->y = y;
    }

    inline Point(const CvPoint &p)
    {
        x = p.x;
        y = p.y;
    }

    inline double norm(void) const
    {
        return sqrt(x * x + y * y);
    }

    inline double norm2(void) const
    {
        return x * x + y * y;
    }

    double distanceToSegment(const Segment &seg) const;
};

std::ostream& operator<<(std::ostream &out, const Point &p);

#endif
