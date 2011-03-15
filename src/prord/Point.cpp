#include "Point.h"

#include "Segment.h"

using namespace std;

double Point::distanceToSegment(const Segment &seg) const
{
    Point AC(seg.p1().x - x, seg.p1().y - y);
    Point BC(seg.p2().x - x, seg.p2().y - y);
    Point AB = seg.dP();

    double dAH = (AB.x * AC.x + AB.y * AC.y) / AB.norm();

    if (dAH < 0 || dAH > AB.norm())
    {
        return min(AC.norm(), BC.norm());
    }

    return sqrt(AC.norm2() + dAH * dAH);
}

ostream& operator<<(ostream &out, const Point &p)
{
    out << "(" << p.x << "," << p.y << ")";

    return out;
}
