#include "SolarSystemFactory.h"

#include "SolarSystemInterface.h"
#include "StarInterface.h"
#include "TestingSystem.h"
#include "ProceduralSystem.h"
#include "EmptySystem.hpp"

std::shared_ptr<SolarSystemInterface> SolarSystemFactory::getTestingSystem() {
    std::shared_ptr<SolarSystemInterface> s = 0;
    s = std::make_shared<TestingSystem>();
    return s;
}

std::shared_ptr<SolarSystemInterface> SolarSystemFactory::getStarSystem(StarInterface * star) {
    std::shared_ptr<SolarSystemInterface> s = 0;
    s = std::make_shared<ProceduralSystem>(star);
    return s;
}

std::shared_ptr<SolarSystemInterface> SolarSystemFactory::getEmptySystem() {
    std::shared_ptr<SolarSystemInterface> s = 0;
    s = std::make_shared<EmptySystem>();
    return s;
}