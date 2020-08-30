#ifndef ENGINE_SYSTEM_INTERFACE_H_
#define ENGINE_SYSTEM_INTERFACE_H_

#include "ShipSystemBase.h"

#include <Box2D/Box2D.h>
#include <assert.h>
#include <functional>

class EngineSystemInterface : public ShipSystemBase {
public:
    virtual ~EngineSystemInterface() {}
    // ESI_01
    virtual double Refuel(double value) = 0;
    // ESI_02
    virtual double FuelMass() = 0;
    // ESI_03
    virtual void Mount(b2Body *body) = 0;
};

#endif // ENGINE_SYSTEM_INTERFACE_H_
