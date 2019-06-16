#ifndef UNIVERSE_INTERFACE_H_
#define UNIVERSE_INTERFACE_H_

#include "StarInterface.h"

#include <vector>

typedef std::vector<StarInterface *> StarCollectionType;

class UniverseInterface {
public:
    virtual ~UniverseInterface() {}
    virtual void GetStars(const double & centerX, const double & centerY, const double & distance, StarCollectionType & stars) = 0;
};

#endif // UNIVERSE_INTERFACE_H_
