#ifndef SOLAR_SYSTEM_FACTORY_H_
#define SOLAR_SYSTEM_FACTORY_H_

#include <memory>

class SolarSystemInterface;

class SolarSystemFactory {
public:
    static std::shared_ptr<SolarSystemInterface> getTestingSystem();
};

#endif // SOLAR_SYSTEM_FACTORY_H_
