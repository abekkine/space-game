#ifndef ENGINE_SYSTEM_INTERFACE_H_
#define ENGINE_SYSTEM_INTERFACE_H_

#include "ShipSystemBase.h"

#include <Box2D.h>
#include <assert.h>
#include <functional>

class EngineSystemInterface : public ShipSystemBase {
public:
    virtual ~EngineSystemInterface() {}

public:
    // Fuel stuff
    virtual double Refuel(double value) = 0;
    virtual double FuelMass() = 0;
    virtual void Mount(b2Body *body) = 0;
};

#endif // ENGINE_SYSTEM_INTERFACE_H_
