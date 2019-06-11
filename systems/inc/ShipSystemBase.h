#ifndef SHIP_SYSTEM_BASE_H_
#define SHIP_SYSTEM_BASE_H_

#include "BusDataTypes.h"
#include "DataBus.h"
#include "DataBusConnection.h"

#include <string>

#include <assert.h>

class ShipSystemBase {
public:
    virtual ~ShipSystemBase() {}
    virtual void Enable() {
        active_ = true;
    }
    virtual void Disable() {
        active_ = false;
    }
    virtual void Init(DataBus* bus) {
        assert(bus != 0);
        bus_ = bus;
        bus_connection_ = bus_->Connect(name_);
    }
    virtual void Update(double time_step) {
        (void)time_step;
    }
    virtual void Disconnect() {
        bus_->Disconnect(name_, bus_connection_);
    }

protected:
    std::string name_;
    bool active_;
    DataBus* bus_;
    DataBusConnection* bus_connection_;
};

#endif // SHIP_SYSTEM_BASE_H_
