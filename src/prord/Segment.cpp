#include "Segment.h"

#include <cmath>
#include <cv.h>

using namespace std;

Segment::Segment(const Point &p1, const Point &p2)
{
    update(p1, p2);
}

Segment::Segment(double rho, double theta, double thetaMin, double thetaMax)
{
    update(rho, theta, thetaMin, thetaMax);
}

void Segment::update(const Point &p1, const Point &p2)
{
    m_p1 = p1;
    m_p2 = p2;

    Point dP(p1.x - p2.x, p1.y - p2.y);
    m_theta = atan(dP.y / (double)dP.x) + CV_PI / 2;

    // http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/
    double u = (p1.x * (p1.x  - p2.x) + p1.y * (p1.y - p2.y)) / dP.norm2();
    Point proj(p1.x + u * (p2.x - p1.x), p1.y + u * (p2.y - p1.y));
    m_rho = (proj.y < 0 ? -1 : 1) * proj.norm();

    double theta1 = atan(p1.y / (double)p1.x);
        if (theta1 < 0) { theta1 += CV_PI; }
    double theta2 = atan(p2.y / (double)p2.x);
        if (theta2 < 0) { theta2 += CV_PI; }
    m_thetaMin = min(theta1, theta2);
    m_thetaMax = max(theta1, theta2);
}

void Segment::update(double rho, double theta, double thetaMin, double thetaMax)
{
    m_rho = rho;
    m_theta = theta;
    m_thetaMin = thetaMin;
    m_thetaMax = thetaMax;

    double angle = theta - thetaMin;
    double hyp = m_rho / cos(angle);
    m_p1 = Point(hyp * cos(thetaMin), hyp * sin(thetaMin));

    angle = thetaMax - theta;
    hyp = m_rho / cos(angle);
    m_p2 = Point(hyp * cos(thetaMax), hyp * sin(thetaMax));
}

ostream& operator<<(ostream &out, const Segment &s)
{
    out << s.p1() << "," << s.p2() << "||";
    out << "(" << s.rho() << "," << s.theta() << ",";
    out << s.thetaMin() << "," << s.thetaMax() << ")";

    return out;
}
