#ifndef DETECTION_OBJECT_H_
#define DETECTION_OBJECT_H_

struct DetectionObject {
    DetectionObject(double c, double h)
    : center(c)
    , horizon(h)
    {}
    double center, horizon;
};

#endif // DETECTION_OBJECT_H_
