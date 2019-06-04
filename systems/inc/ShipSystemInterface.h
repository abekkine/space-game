#ifndef SHIP_SYSTEM_INTERFACE_H_
#define SHIP_SYSTEM_INTERFACE_H_

#include "DataBus.h"
#include "DataBusConnection.h"

class ShipSystemInterface {
public:
    virtual void Init(DataBus* bus) = 0;
    virtual void Update(double time_step) = 0;
    // NOTE : Also providing a Connect() method here would be proper (#114).
    virtual void Disconnect() = 0;
public:
    virtual void Enable() { active_ = true; }
    virtual void Disable() { active_ = false; }
protected:
    bool active_;
    DataBus* bus_;
    DataBusConnection* bus_connection_;
};

#endif // SHIP_SYSTEM_INTERFACE_H_

