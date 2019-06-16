#ifndef STAR_CATEGORY_H_
#define STAR_CATEGORY_H_

#include <string>

enum StarSequenceType {
    sst_None,
    sst_MainSequence,
    sst_Giant,
    sst_SuperGiant,
};

struct StarCategory {
    std::string name;
    StarSequenceType type;
    double abundance;
    float * baseColor;
    double minRadius;
    double maxRadius;
    double minMass;
    double maxMass;
};

enum {
    e_NUM_CATEGORIES = 23,
};

#endif // STAR_CATEGORY_H_
