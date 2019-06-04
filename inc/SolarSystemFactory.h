#ifndef SOLAR_SYSTEM_FACTORY_H_
#define SOLAR_SYSTEM_FACTORY_H_

class SolarSystemInterface;

class SolarSystemFactory {
public:
    static SolarSystemInterface * getTestingSystem();
};

#endif // SOLAR_SYSTEM_FACTORY_H_
