#ifndef HULL_SYSTEM_INTERFACE_H_
#define HULL_SYSTEM_INTERFACE_H_

#include "ShipSystemBase.h"

#include <functional>

#include <assert.h>

class DataBus;
class DataBusConnection;

class HullSystemInterface : public ShipSystemBase {
public:
    virtual ~HullSystemInterface() {}

public:
    virtual double Repair(double value) = 0;
    virtual void ApplyImpact(double impulse) = 0;
    virtual void SetDestructionCallback(std::function<void()> cb) = 0;

protected:
    std::function<void()> on_destroyed_;
};

#endif // HULL_SYSTEM_INTERFACE_H_

