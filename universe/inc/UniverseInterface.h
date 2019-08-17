#ifndef UNIVERSE_INTERFACE_H_
#define UNIVERSE_INTERFACE_H_

#include "StarCollection.hpp"

#include <vector>
#include <memory>

class UniverseInterface {
public:
    virtual ~UniverseInterface() {}
    virtual std::shared_ptr<StarCollection> GetStars() = 0;
    virtual void UpdateStars(const double & centerX, const double & centerY, const double & distance) = 0;
    virtual void * GetParameters() = 0;
    virtual void Save() = 0;
    virtual void Load() = 0;
};

#endif // UNIVERSE_INTERFACE_H_
