#ifndef UNIVERSE_FACTORY_HPP
#define UNIVERSE_FACTORY_HPP

#include "UniverseInterface.h"
#include "Universe.h"
#include "UniverseV2.hpp"

class UniverseFactory {
public:
    static UniverseInterface * getUniverse(const std::string & desc) {
        UniverseInterface * uni = 0;
        if (desc == "norm") {
            uni = new Universe();
        }
        else if (desc == "v2") {
            uni = new UniverseV2();
        }
        return uni;
    }
};

#endif // UNIVERSE_FACTORY_HPP
