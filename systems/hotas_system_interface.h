#ifndef HOTAS_SYSTEM_INTERFACE_H_
#define HOTAS_SYSTEM_INTERFACE_H_

#include "ship_system_interface.h"

#include <assert.h>

class HotasSystemInterface : public ShipSystemInterface {
public:
    virtual ~HotasSystemInterface() {}
public:
    virtual void Init(DataBus * bus) {
        assert(bus != 0);
        bus_ = bus;

        bus_connection_ = bus_->Connect("hotas");
    }
    virtual void Update(double time_step) { (void)time_step; }
    virtual void Disconnect() {
        bus_->Disconnect("hud", bus_connection_);
    }
};

#endif // HOTAS_SYSTEM_INTERFACE_H_

