#ifndef SHIP_DEVICE_H_
#define SHIP_DEVICE_H_

#include "data_bus.h"

class ShipDevice {
public:
    virtual ~ShipDevice() {}
    // TODO : Instead of init Connect / Disconnect (bus) methods would be provided (#114).
    virtual void Init(DataBus* bus) = 0;
protected:
    DataBus * bus_;
    DataBus::Connection * bus_connection_;
};

#endif // SHIP_DEVICE_H_
