#include "SolarSystemFactory.h"

#include "SolarSystemInterface.h"
#include "TestingSystem.h"

std::shared_ptr<SolarSystemInterface> SolarSystemFactory::getTestingSystem() {
    std::shared_ptr<SolarSystemInterface> s = 0;
    s = std::make_shared<TestingSystem>();
    return s;
}
