#ifndef SOLAR_SYSTEM_FACTORY_H_
#define SOLAR_SYSTEM_FACTORY_H_

#include "solar_system_interface.h"
#include "solar_system.h"

class SolarSystemFactory {
public:
    static SolarSystemInterface * getInitialSystem() {
        SolarSystemInterface * s = 0;
        s = new SolarSystem();
        return s;
    }
};

#endif // SOLAR_SYSTEM_FACTORY_H_
