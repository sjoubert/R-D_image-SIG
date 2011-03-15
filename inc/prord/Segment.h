#ifndef SJ_SEGMENT_H_05122010212447
#define SJ_SEGMENT_H_05122010212447

#include <iostream>

#include "Point.h"

class Segment
{
    public:
        Segment(const Point &p1, const Point &p2);
        Segment(double rho, double theta, double thetaMin, double thetaMax);

        inline Point p1(void) const { return m_p1; }
        inline Point p2(void) const { return m_p2; }
        inline double rho(void) const { return m_rho; }
        inline double theta(void) const { return m_theta; }
        inline double thetaMin(void) const { return m_thetaMin; }
        inline double thetaMax(void) const { return m_thetaMax; }
        inline Point dP(void) const { return Point(m_p2.x - m_p1.x, m_p2.y - m_p1.y); };

    private:
        void update(const Point &p1, const Point &p2);
        void update(double rho, double theta, double thetaMin, double thetaMax);

        // Coordonnees cartesiennes
        Point m_p1;
        Point m_p2;

        // Coordonnees normales
        double m_rho, m_theta; // c R; [0;Pi]
        double m_thetaMin, m_thetaMax;
};

std::ostream& operator<<(std::ostream &out, const Segment &s);

#endif
