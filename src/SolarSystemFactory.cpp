#include "SolarSystemFactory.h"

#include "SolarSystemInterface.h"
#include "TestingSystem.h"

SolarSystemInterface * SolarSystemFactory::getTestingSystem() {
    SolarSystemInterface * s = 0;
    s = new TestingSystem();
    return s;
}
