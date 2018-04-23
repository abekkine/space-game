#ifndef HUD_SYSTEM_INTERFACE_H_
#define HUD_SYSTEM_INTERFACE_H_

#include "ship_system_interface.h"

#include <assert.h>

class HudSystemInterface : public ShipSystemInterface {
public:
    virtual ~HudSystemInterface() {}
    virtual void Render() = 0;
public:
    virtual void Init(DataBus * bus) {
        assert(bus != 0);
        bus_ = bus;

        bus_connection_ = bus_->Connect("hud");
    }
    virtual void Update(double time_step) { (void)time_step; }
    virtual void Disconnect() {
        bus_->Disconnect("hud", bus_connection_);
    }
};

#endif // HUD_SYSTEM_INTERFACE_H_

