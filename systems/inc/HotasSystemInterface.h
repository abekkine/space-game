#ifndef HOTAS_SYSTEM_INTERFACE_H_
#define HOTAS_SYSTEM_INTERFACE_H_

#include "ShipSystemInterface.h"
#include "EngineSystemInterface.h"

#include <assert.h>

class HotasSystemInterface : public ShipSystemInterface {
public:
    virtual ~HotasSystemInterface() {}
    virtual void SetThrottle(double value) = 0;
    virtual void SetSteering(double value) = 0;
    virtual void Stabilize() = 0;
    virtual void ToggleLandingGear() = 0;

public:
    virtual void Init(DataBus * bus) {
        assert(bus != 0);
        bus_ = bus;

        bus_connection_ = bus_->Connect("hotas");
    }
    virtual void Update(double time_step) {
        (void)time_step;
    }
    virtual void Disconnect() {
        bus_->Disconnect("hud", bus_connection_);
    }

protected:
    EngineSystemInterface* ship_engine_;
};

#endif // HOTAS_SYSTEM_INTERFACE_H_

