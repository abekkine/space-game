#ifndef SOLAR_SYSTEM_INTERFACE_H_
#define SOLAR_SYSTEM_INTERFACE_H_

#include <Box2D.h>

class SolarSystemInterface {
public:
    virtual ~SolarSystemInterface() {}
    virtual void Init(b2World * world) = 0;
    virtual void Render() = 0;
    virtual void Update(double time_step) = 0;
};

#endif // SOLAR_SYSTEM_INTERFACE_H_
