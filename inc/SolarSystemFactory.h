#ifndef SOLAR_SYSTEM_FACTORY_H_
#define SOLAR_SYSTEM_FACTORY_H_

#include <memory>

class SolarSystemInterface;
class StarInterface;

class SolarSystemFactory {
public:
    static std::shared_ptr<SolarSystemInterface> getTestingSystem();
    static std::shared_ptr<SolarSystemInterface> getStarSystem(StarInterface * star);
    static std::shared_ptr<SolarSystemInterface> getEmptySystem();
};

#endif // SOLAR_SYSTEM_FACTORY_H_
