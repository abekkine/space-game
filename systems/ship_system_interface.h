#ifndef SHIP_SYSTEM_INTERFACE_H_
#define SHIP_SYSTEM_INTERFACE_H_

#include "data_bus.h"

class ShipSystemInterface {
public:
    virtual void Init(DataBus* bus) = 0;
    virtual void Update(double time_step) = 0;
    // TODO : Also providigin a Connect() method here would be proper.
    virtual void Disconnect() = 0;
protected:
    DataBus* bus_;
    DataBus::Connection* bus_connection_;
};

#endif // SHIP_SYSTEM_INTERFACE_H_

