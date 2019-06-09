#ifndef ENGINE_SYSTEM_INTERFACE_H_
#define ENGINE_SYSTEM_INTERFACE_H_

#include "ShipSystemInterface.h"

#include <Box2D.h>
#include <assert.h>
#include <functional>

class EngineSystemInterface : public ShipSystemInterface {
public:
    virtual ~EngineSystemInterface() {}

public:
    // Pilot commands
    virtual void ThrustForwardsCommand(double value) = 0;
    virtual void MomentCcwCommand(double value) = 0;
    virtual void MomentCwCommand(double value) = 0;
    virtual void CancelMomentCommand() = 0;
    // Fuel stuff
    virtual double Refuel(double value) = 0;
    virtual double DumpFuel(double value) = 0;
    virtual double FuelQuery() = 0;
    virtual double FuelMass() = 0;

    virtual void Mount(b2Body *body) = 0;

    // Standard ship system interface
    virtual void Init(DataBus* bus) {
        assert(bus != 0);
        bus_ = bus;
        bus_connection_ = bus_->Connect("engine");
    }
    virtual void Update(double time_step) {
        (void)time_step;
    }
    virtual void Disconnect() {
        bus_->Disconnect("engine", bus_connection_);
    }
};

#endif // ENGINE_SYSTEM_INTERFACE_H_
