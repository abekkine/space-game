#ifndef SOLAR_SYSTEM_FACTORY_H_
#define SOLAR_SYSTEM_FACTORY_H_

#include "SolarSystemInterface.h"
#include "TestingSystem.h"

class SolarSystemFactory {
public:
    static SolarSystemInterface * getTestingSystem() {
        SolarSystemInterface * s = 0;
        s = new TestingSystem();
        return s;
    }
};

#endif // SOLAR_SYSTEM_FACTORY_H_
