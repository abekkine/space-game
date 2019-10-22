#ifndef SOLAR_SYSTEM_FACTORY_H_
#define SOLAR_SYSTEM_FACTORY_H_

#include <memory>

#include <Box2D.h>

class SolarSystemInterface;

class SolarSystemFactory {
public:
    static std::shared_ptr<SolarSystemInterface> getTestingSystem(b2World * world);
    static std::shared_ptr<SolarSystemInterface> getStarSystem(b2World * world);
    static std::shared_ptr<SolarSystemInterface> getEmptySystem(b2World * world);
};

#endif // SOLAR_SYSTEM_FACTORY_H_
