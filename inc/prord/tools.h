#ifndef SJ_TOOLS_H_05122010193029
#define SJ_TOOLS_H_05122010193029

#include <vector>

#include <cv.h>

#include "Segment.h"

std::vector<Segment> convert(CvSeq *lines);

void draw(const std::vector<Segment> &lines, IplImage *img);

void merge(std::vector<Segment> &lines, int threshold, double toleranceRad);

Segment doMerge(const Segment &tgt, const Segment &src);

bool areClose(const Segment &s1, const Segment &s2, int threshold);

#endif
