#ifndef UNIVERSE_FACTORY_HPP
#define UNIVERSE_FACTORY_HPP

#include "UniverseInterface.h"
#include "Universe.h"

class UniverseFactory {
public:
    static UniverseInterface * getUniverse(const std::string & desc) {
        UniverseInterface * uni = 0;
        if (desc == "norm") {
            uni = new Universe();
        }
        return uni;
    }
};

#endif // UNIVERSE_FACTORY_HPP
