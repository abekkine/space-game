#ifndef HULL_SYSTEM_INTERFACE_H_
#define HULL_SYSTEM_INTERFACE_H_

#include "ShipSystemInterface.h"

#include <functional>

#include <assert.h>

class DataBus;
class DataBusConnection;

class HullSystemInterface : public ShipSystemInterface {
public:
    virtual ~HullSystemInterface() {}

public:
    virtual void Init(DataBus * bus) {
        assert(bus != 0);
        bus_ = bus;
        bus_connection_ = bus_->Connect("hull");
    }
    virtual void Update(double time_step) {
        (void)time_step;
    } 
    virtual void Disconnect() {
        bus_->Disconnect("hull", bus_connection_);
    }

public:
    virtual double Repair(double value) = 0;
    virtual void ApplyImpact(double impulse) = 0;
    virtual void SetDestructionCallback(std::function<void()> cb) = 0;

protected:
    std::function<void()> on_destroyed_;
};

#endif // HULL_SYSTEM_INTERFACE_H_

