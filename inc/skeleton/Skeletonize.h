#ifndef SJ_SKELETONIZE_H_22022011142139
#define SJ_SKELETONIZE_H_22022011142139

#include <list>

#include "cv.h"

class Skeletonize
{
    public:
        Skeletonize(IplImage *src);
        void operator()(void);

    private:
        IplImage *m_src;
        static const int s_lut[256];

        int thin(unsigned int pass);
};

#endif
