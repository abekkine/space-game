#ifndef DETECTION_OBJECT_H_
#define DETECTION_OBJECT_H_

struct DetectionObject {
    DetectionObject(double c, double s)
    : center(c)
    , span(s)
    {}
    double center, span;
};

#endif // DETECTION_OBJECT_H_
