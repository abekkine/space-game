#include "SolarSystemFactory.h"

#include "SolarSystemInterface.h"
#include "StarInterface.h"
#include "TestingSystem.h"
#include "ProceduralSystem.h"
#include "EmptySystem.hpp"

std::shared_ptr<SolarSystemInterface> SolarSystemFactory::getTestingSystem(b2World * world) {
    std::shared_ptr<SolarSystemInterface> s = 0;
    s = std::make_shared<TestingSystem>(world);
    return s;
}

std::shared_ptr<SolarSystemInterface> SolarSystemFactory::getStarSystem(b2World * world) {
    std::shared_ptr<SolarSystemInterface> s = 0;
    s = std::make_shared<ProceduralSystem>(world);
    return s;
}

std::shared_ptr<SolarSystemInterface> SolarSystemFactory::getEmptySystem(b2World * world) {
    std::shared_ptr<SolarSystemInterface> s = 0;
    s = std::make_shared<EmptySystem>();
    return s;
}